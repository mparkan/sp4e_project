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
 * This class represents a circular buffer.
 * 
 */
 
#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <array>

class CircularBuffer {

friend class CircularBufferCollection;
friend class PointCollection;
friend class SegmenterSNC;

public:

	unsigned int GetSize();
	unsigned int GetRadius();
	
	/**
	 * Creates a circular buffer.
	 *
	 * @param  radius The radius of the circular buffer.
	 * @param  scaling_factor The coordinate scaling factor (1 metric, 10 = decimetric). Defaults to 1, if different values than 1 or 10 are given.
	 */
	CircularBuffer(unsigned int radius, unsigned int scaling_factor); // Constructor
	~CircularBuffer(){}; // Destructor
	
private:
	
	/**
	 * Coordinate offsets representing a circular buffer area with regard to a grid cell.
	 *
	 */
	std::vector<std::array<int,2>> coordinate_offsets_;
	unsigned int radius_;
	unsigned int size_;
	unsigned int scaling_factor_;
	
};

#endif
