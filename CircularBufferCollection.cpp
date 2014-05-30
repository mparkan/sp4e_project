#include <vector>
#include "PointCollection.h"
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"

using namespace std;


CircularBuffer& CircularBufferCollection::GetCircularBuffer(unsigned int k)
{
	return circular_buffers_[k];
	
}


//CircularBufferCollection::CircularBufferCollection(vector<unsigned int> radius_list, PointCollection point_collection)
CircularBufferCollection::CircularBufferCollection(vector<unsigned int> radius_list, unsigned int scaling_factor)
{
	
	for (unsigned int j = 0; j < radius_list.size(); j++){
		
		//circular_buffers_.push_back(CircularBuffer(radius_list[j], point_collection.coordinate_scaling_));
		circular_buffers_.push_back(CircularBuffer(radius_list[j], scaling_factor));
		
	}
	
}
