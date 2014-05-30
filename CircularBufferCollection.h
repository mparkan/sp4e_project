/**
 * @file
 * @author  Matthew Parkan <matthew.parkan@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This class represents a collection of circular buffers.
 * 
 */
 
#ifndef CIRCULARBUFFERCOLLECTION_H
#define CIRCULARBUFFERCOLLECTION_H

#include <vector>
#include "CircularBuffer.h"
#include "PointCollection.h"

class CircularBufferCollection {

friend class SegmenterSNC;

public:
	
	CircularBuffer& GetCircularBuffer(unsigned int k);
		
	/**
	 * Creates a collection of circular buffers.
	 *
	 * @param  radius_list A list of radius for which to create a CircularBuffer.
	 * @param  scaling_factor The coordinate scaling factor (1 metric, 10 = decimetric). Defaults to 1, if different values than 1 or 10 are given.
	 */
	CircularBufferCollection(std::vector<unsigned int> radius_list, unsigned int scaling_factor); // Constructor
	~CircularBufferCollection(){}; // Destructor
	
	
private:

	std::vector<CircularBuffer> circular_buffers_;
	unsigned int scaling_factor_;
	
};

#endif
