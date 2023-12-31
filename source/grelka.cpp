/*
 code is Public Domain and written by Peter Semiletov, 2023
*/

#include "lv2/core/lv2.h"


#include <cmath>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "dsp.h"
#include "fx-resofilter.h"

#define GRELKA_URI "https://github.com/psemiletov/grelka"

typedef enum { MTL_INPUT = 0, MTL_OUTPUT = 1, MTL_DRIVE = 2, MTL_LEVEL = 3, MTL_HPF = 4, MTL_LPF = 5} PortIndex;



class CGrelka
{
public:

  int samplerate;

  CResoFilter hp;
  CResoFilter lp;

  const float* drive;
  const float* level;
  const float* lpf;
  const float* hpf;

  const float* input;
  float *output;

  CGrelka();
};


CGrelka::CGrelka()
{
  hp.mode = FILTER_MODE_HIGHPASS;
  lp.reset();
  hp.reset();
}


static void
activate(LV2_Handle instance)
{}


static LV2_Handle
instantiate (const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  init_db();
  CGrelka *instance = new CGrelka;
  instance->samplerate = rate;

  return (LV2_Handle)instance;
}


static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
  CGrelka *inst = (CGrelka*)instance;

  switch ((PortIndex)port)
         {
          case MTL_INPUT:
                          inst->input = (const float*)data;
                          break;

          case MTL_OUTPUT:
                          inst->output = (float*)data;
                          break;

          case MTL_DRIVE:
                          inst->drive = (const float*)data;
                          break;

          case MTL_LEVEL:
                          inst->level = (const float*)data;
                          break;

          case MTL_LPF:
                          inst->lpf = (const float*)data;
                          break;

          case MTL_HPF:
                          inst->hpf = (const float*)data;
                          break;

        }
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
  CGrelka *inst = (CGrelka*)instance;

  inst->lp.set_cutoff ((float) *(inst->lpf) / inst->samplerate);
  inst->hp.set_cutoff ((float) *(inst->hpf) / inst->samplerate);

  for (uint32_t pos = 0; pos < n_samples; pos++)
      {
       float f = inst->input[pos];

       f = overdrive (f, *(inst->drive), db2lin (*(inst->level)));

       f = inst->lp.process (f);
       f = inst->hp.process (f);

       inst->output[pos] = f;
      }
}


static void deactivate (LV2_Handle instance)
{
}


static void cleanup(LV2_Handle instance)
{
  delete (CGrelka*)instance;
}


static const void* extension_data (const char* uri)
{
  return NULL;
}


static const LV2_Descriptor descriptor = {GRELKA_URI,
                                          instantiate,
                                          connect_port,
                                          activate,
                                          run,
                                          deactivate,
                                          cleanup,
                                          extension_data};


LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
  return index == 0 ? &descriptor : NULL;
}
