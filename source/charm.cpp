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

#define METALLUGA_URI "https://github.com/psemiletov/charm"

typedef enum { PORT_IN_LEFT = 0, PORT_IN_RIGHT = 1, PORT_OUT_LEFT = 2, PORT_OUT_RIGHT = 3, PORT_CHARM = 4} PortIndex;



class CCharm
{
public:

  int samplerate;


  const float* charm;

  const float* input_l;
  const float* input_r;

  float *output_l;
  float *output_r;

  //CCharm();
};




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
  CCharm *instance = new CCharm;
  instance->samplerate = rate;

  return (LV2_Handle)instance;
}


static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
  CCharm *inst = (CCharm*)instance;

  switch ((PortIndex)port)
         {
          case PORT_IN_LEFT:
                          inst->input_l = (const float*)data;
                          break;

          case PORT_IN_RIGHT:
                          inst->input_r = (const float*)data;
                          break;

          case PORT_OUT_LEFT:
                          inst->output_l = (float*)data;
                          break;

          case PORT_OUT_RIGHT:
                          inst->output_r = (float*)data;
                          break;

          case PORT_CHARM:
                          inst->charm = (const float*)data;
                          break;


        }
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
  CCharm *inst = (CCharm*)instance;

  //const float* const input  = inst->input;
  //float* const       output = inst->output;

  for (uint32_t pos = 0; pos < n_samples; pos++)
      {
       float fl = inst->input_l[pos];
       float fr = inst->input_r[pos];

       fl = warmify (fl, *(inst->charm));
       fr = warmify (fr, *(inst->charm));

       inst->output_l[pos] = fl;
       inst->output_r[pos] = fr;
      }
}


static void deactivate (LV2_Handle instance)
{
}


static void cleanup(LV2_Handle instance)
{
  delete (CCharm*)instance;
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
