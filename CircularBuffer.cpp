#include <vector>
#include "CircularBuffer.h"

using namespace std;

unsigned int CircularBuffer::GetRadius()
{
	return radius_;
}


unsigned int CircularBuffer::GetSize()
{
  return size_;	
}


CircularBuffer::CircularBuffer(unsigned int radius, unsigned int scaling_factor)
{
	radius_ = radius;
	scaling_factor_ = scaling_factor;
	
	unsigned int scaled_radius = scaling_factor*radius; 
	unsigned int kernel_size = (2*scaled_radius)+1;
	int squared_radius = scaled_radius*scaled_radius;
	
	// Create circular kernel index offsets
	for (unsigned int j = 0; j < kernel_size; j++){
		
		for (unsigned int k = 0; k < kernel_size; k++){
			
			int xx = j-scaled_radius; // Column offset
			int yy = k-scaled_radius; // Row offset
			
			// Check if the grid point is located within the circle
			if ((xx*xx + yy*yy) <= squared_radius){
				
				coordinate_offsets_.push_back({xx,yy}); 
				
			}
			
		}
		
	}
	
	size_ = coordinate_offsets_.size();

}


