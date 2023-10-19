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

#define METALLUGA_URI "https://github.com/psemiletov/metalluga"

typedef enum { MTL_INPUT = 0, MTL_OUTPUT = 1, MTL_DRIVE = 2, MTL_LEVEL = 3, MTL_WEIGHT = 4, MTL_RESO = 5, MTL_WARMTH = 6} PortIndex;



class CMetalluga
{
public:

  int samplerate;

  //CResoFilter hp_pre;


  CResoFilter lp;
  CResoFilter hp;

  const float* drive;
  const float* level;
  const float* weight;
  const float* reso;
  const float* warmth;

  const float* input;
  float *output;

  CMetalluga();
};


CMetalluga::CMetalluga()
{
  hp.mode = FILTER_MODE_HIGHPASS;
  lp.reset();
  hp.reset();
  //hp_pre.mode = FILTER_MODE_HIGHPASS;

//  hp_pre.reset();
}


static void
activate(LV2_Handle instance)
{}


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  init_db();
  CMetalluga *instance = new CMetalluga;
  instance->samplerate = rate;
//  instance->hp_pre.set_cutoff ((float) 7.2f / rate);

  return (LV2_Handle)instance;
}


static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
  CMetalluga *inst = (CMetalluga*)instance;

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

          case MTL_WEIGHT:
                          inst->weight = (const float*)data;
                          break;

          case MTL_RESO:
                          inst->reso = (const float*)data;
                          break;

          case MTL_WARMTH:
                          inst->warmth = (const float*)data;
                          break;

        }
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
  CMetalluga *inst = (CMetalluga*)instance;

  const float* const input  = inst->input;
  float* const       output = inst->output;

  for (uint32_t pos = 0; pos < n_samples; pos++)
      {

       float f = input[pos];

       f *= db2lin (*(inst->level));

       f = gritty_guitar_distortion(f, *(inst->drive));

       inst->lp.set_cutoff (1 - *(inst->weight));
       inst->hp.set_cutoff (1 - *(inst->weight));

       f = inst->lp.process (f);
       f = inst->hp.process (f);

       f = apply_resonance (f, *(inst->reso));

       f = warmify (f, *(inst->warmth));

       output[pos] = f;
   }
}


static void deactivate (LV2_Handle instance)
{
}


static void cleanup(LV2_Handle instance)
{
  delete (CMetalluga*)instance;
}


static const void* extension_data (const char* uri)
{
  return NULL;
}


static const LV2_Descriptor descriptor = {METALLUGA_URI,
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
