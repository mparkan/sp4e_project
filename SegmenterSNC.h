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
 * This class applies the sequential nearest cluster segmentation algorithm proposed in [1] to a PointCollection.
 * 
 * [1]W. Li, Q. Guo, M. K. Jakubowski, and M. Kelly, 
 * “A new method for segmenting individual trees from the lidar point cloud,” 
 * Photogrammetric Engineering and Remote Sensing, vol. 78, no. 1, pp. 75–84, 2012.
 * 
 */
 
#ifndef SEGMENTERSNC_H
#define SEGMENTERSNC_H
#include <iostream>
#include <vector>
#include "PointCollection.h"
#include "SegmenterSNC.h"
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"


class SegmenterSNC {

friend class PointCollection;

public:
	
	/**
	 * Attempts to split the PointCollection into groups each representing an individual tree.
	 *
	 * @param  point_collection A reference to the PointCollection which is to be segmented.
	 * @param  circular_buffer_collection A reference to the CircularBufferCollection used to extract Points within the local neighbourhood around the suspected tree.
	 * @param  verbosity If true, will print information about the segmentation process to the terminal. 
	 */
	void SegmentPointCollection(PointCollection& point_collection, CircularBufferCollection& circular_buffer_collection, bool verbosity);
	
	SegmenterSNC(){}; // Constructor
	~SegmenterSNC(){}; // Destructor
	
private:
	
	/**
	 * Number of unsegmented points.
	 *
	 */
	unsigned int n_unsegmented_;

	/**
	 * Classifies a sample PointCollection into groups P (part of the the tree) and N (not part of the tree).
	 *
	 * @param  P A reference to the PointCollection where Points that are part of the tree will be contained.
	 * @param  N A reference to the PointCollection where Points that are not part of the tree will be contained.
	 * @param  sample A reference to the PointCollection where each Point within the CircularBuffer will be contained.
	 */
	void ClassifySample(PointCollection& P, PointCollection& N, PointCollection& sample);
	
	
	/**
	 * Finds the minimum squared distance between the specified Point and all the Points within the specified PointCollection.
	 *
	 * @param  point A reference to a Point.
	 * @param  point_collection A reference to the a PointCollection.
	 * @return Returns the smallest squared distance between the specified Point and all the Points within the specified PointCollection.
	 */
	double FindMinDistance(PointCollection::Point& point, PointCollection& point_collection);
	
};

#endif
