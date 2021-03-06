// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../core/differential_geometry.h"

namespace embree {

struct Light;

struct Light_SampleRes
{
  Vec3fa weight;  //!< radiance that arrives at the given point divided by pdf
  Vec3fa dir;     //!< direction towards the light source
  float dist;    //!< largest valid t_far value for a shadow ray
  float pdf;     //!< probability density that this sample was taken
};

//! compute the weighted radiance at a point caused by a sample on the light source
// by convention, giving (0, 0) as "random" numbers should sample the "center"
// of the light source (used by the raytracing renderers such as the OBJ renderer)
typedef Light_SampleRes (*Light_SampleFunc)(const Light* self,
                                            const DifferentialGeometry& dg, /*! point to generate the sample for >*/
                                            const Vec2f& s);                /*! random numbers to generate the sample >*/


struct Light_EvalRes
{
  Vec3fa value;     //!< radiance that arrives at the given point (not weighted by pdf)
  float dist;
  float pdf;       //!< probability density that the direction would have been sampled
};

//! compute the radiance, distance and pdf caused by the light source (pointed to by the given direction)
typedef Light_EvalRes (*Light_EvalFunc)(const Light* self,
                                        const DifferentialGeometry& dg, /*! point to evaluate illumination for >*/
                                        const Vec3fa& dir);              /*! direction towards the light source >*/


struct Light
{
  Light_SampleFunc sample;
  Light_EvalFunc eval;
};

Light_EvalRes Light_eval(const Light* self, const DifferentialGeometry& dg, const Vec3fa& dir);

inline void Light_Constructor(Light* self)
{
  self->eval = Light_eval;
}

extern "C" void Light_destroy(const Light* light);

} // namespace embree
