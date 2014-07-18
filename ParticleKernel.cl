/**********************************************************************
Copyright �2013 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

�	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
�	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

/*!
 * Sample kernel which multiplies every element of the input array with
 * a constant and stores it at the corresponding output array
 */

float4 reflect( float4 i, float4 n )
{
  return i - 2.0 * n * dot(n,i);
}

__constant float MIN_DIST = .1;
__constant float REPELL_RADIUS = .1;

__kernel void templateKernel(__global  float4 *data_buffer,
                             float dt,
							 unsigned long age,
							 unsigned long particleCount,
							 __global float4 *grav_obj_buffer,
							 unsigned long grav_obj_count)
{
    uint tid = get_global_id(0);
    float mass 		= data_buffer[3*tid+2].x;

    global float4* velocity = &data_buffer[3*tid+1];
    global float4* position = &data_buffer[3*tid];

	float4 normal = (float4)(0,1,0,0);
    	float4 force = (float4)(0,0,0,0); //(0,-9.81*mass,0,0);	//
	if ((*position).y < 0) {
		force *=0;	
	}

	bool to_plane = (dot(*velocity,normal)<0);

	float4 grav_force = (float4)(0,0,0,0);
	for (size_t i=0;i<grav_obj_count;i++) {
		float4 ab = grav_obj_buffer[i]-(*position);
		float grav_obj_mass = ab.w;
		ab.w = 0;

		float dist = fast_length(ab);
		
		if (dist < MIN_DIST) {
			dist = MIN_DIST;
		}

		float4 f = .5*grav_obj_mass*mass*normalize(ab)/(dist*dist);

		if (dist < REPELL_RADIUS) {
			f = -f;
		}
		grav_force += f;
	}
	force += grav_force; 
	
	//float4 friction_force = .5*(*velocity);
	//force -= friction_force;
	
	//if (((*position).y < 0) && to_plane) {
	//	*velocity = reflect(*velocity,normal)*.2;
	//	(*position).y = 0;
	//}
	
	//if (length(*velocity)<0.001) {
	//	*velocity*=0;
	//}
	
	*velocity += force/mass*dt;
	*position += *velocity*dt;
}
