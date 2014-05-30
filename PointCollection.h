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
 * This class is a container for 3D points.
 * 
 */

#ifndef POINTCOLLECTION_H
#define POINTCOLLECTION_H

#include <iostream>
#include <vector>
#include <array>
#include "CircularBuffer.h"
#include "CircularBufferCollection.h"

class PointCollection {
	
friend class SegmenterSNC;
friend class FileIO;
friend class CircularBufferCollection;
friend class TreeCollection;

public:
	
	/**
	 * Sorts the Points in the PointCollection by z (height) in descending order.
	 *
	 */
	void SortByZ();
	
	
	/**
	 * Computes the bounding box of the PointCollection.
	 *
	 */
	void ComputeBoundingBox();
	
	/**
	 * Computes the linear index of each Point in the PointCollection.
	 *
	 */
	void ComputePointIndexes();
	
	/**
	 * Computes a grid coordinate (col, row) for each Point in the PointCollection.
	 *
	 * @param  scaling_factor The coordinate scaling factor (1 metric, 10 = decimetric). Defaults to 1, if different values than 1 or 10 are given.
	 */
	void ComputeGridCoordinates();
	
	
	/**
	 * Assigns each Point to a grid cell.
	 *
	 */
	void AssignGridCells();
	
	
	/**
	 * Extracts all the Points located within the specified CircularBuffer centered at (col_0, row_0) to the specified PointCollection.
	 *
	 * @param  circular_buffer A reference to the CircularBuffer used in the extraction.
	 * @param  sample A reference to a PointCollection where the extracted points will contained.
	 * @param  col_0 The column at which the CircularBuffer is centerer.
	 * @param  row_0 The row at which the CircularBuffer is centerer.
	 * 
	 */
	void ExtractPointsInBuffer(CircularBuffer& circular_buffer, PointCollection& sample, int& col_0, int& row_0);
	
	
	/**
	 * Finds all local maxima in the PointCollection. A Point is considered a local maxima if it's z coordinates is larger than or equal 
	 * to all other point wihtin the CircularBuffer centered on it.
	 *
	 * @param  circular_buffer A reference to the CircularBuffer used in the local maxima definition.
	 * 
	 */
	void FindLocalMaxima(CircularBuffer& circular_buffer);
	
	
	/**
	 * Set the int16 RGB color triplet for each Point in the PointCollection based on its tree index (tree_idx member).
	 *
	 * @param  hsv_colormap A colormap consisting of int16 RGB triplets.
	 * 
	 */
	void SetRGBColors(std::vector<std::array<unsigned int, 3>> hsv_colormap);


	/**
	 * Accessor to the coordinate scaling factor used in gridding.
	 * 
	 */
	unsigned int GetScalingFactor();


	/**
	 * Filter Points in the PointCollection by their classification.
	 *
	 * @param  keep_classes The classes which are kept.
	 * 
	 */
	PointCollection FilterPointsByClass(std::vector<unsigned int>& keep_classes);
	
	
	PointCollection(){coordinate_scaling_ = 1;}; // Constructor
	~PointCollection(){}; // Destructor
	
private:
	
	/**
	 * 3D Point.
	 *
	 */
	struct Point {
	
		double x;
		double y;
		double z;
		int row;
		int col;
		unsigned int classification;
		unsigned int point_idx;
		unsigned int tree_idx;
		unsigned int local_maxima_status;
		bool segmentation_status;
		std::array<unsigned int,3> rgb_color;
	
	};
	
	
	/**
	 * Bounding box representing the horizontal extent of a PointCollection.
	 *
	 */
	struct BoundingBox {
		
		bool availability;
		double width;
		double height;
		double x_min;
		double x_max;
		double y_min;
		double y_max;
		unsigned int x_min_idx;
		unsigned int x_max_idx;
		unsigned int y_min_idx;
		unsigned int y_max_idx;
		
	};
	
	
	/**
	 * Points.
	 *
	 */
	std::vector<Point> points_;
	
	/**
	 * Bounding box.
	 *
	 */
	BoundingBox bounding_box_;
	
	/**
	 * Grid.
	 *
	 */
	std::vector<std::vector<int>> idx_grid_;
	
	/**
	 * Number of grid columns.
	 *
	 */
	unsigned int n_cols_;
	
	/**
	 * Number of grid rows.
	 *
	 */
	unsigned int n_rows_;
	
	/**
	 * Coordinate scaling factor (1 metric, 10 = decimetric, 100 = centimetric, etc...) used in gridding.
	 *
	 */
	unsigned int coordinate_scaling_;
	
	/**
	 * Number of segmented points.
	 *
	 */
	unsigned int n_segmented_;
	
	/**
	 * Converts a linear grid cell index to a subscript coordinate (row, col).
	 *
	 * @param  ncols The number of columns in the grid.
	 * @param  nrows ncols The number of rows in the grid.
	 * @param  idx The linear grid cell index.
	 * @return Returns a coordinate (row, col).
	 */
	std::array<unsigned int,2> IndexToSubscript(unsigned int ncols, unsigned int nrows, unsigned int idx);
	
	/**
	 * Converts a subscript coordinate (row, col) to a linear grid cell index.
	 *
	 * @param  ncols The number of columns in the grid.
	 * @param  row The row of the Point.
	 * @param  column The column of the Point.
	 * @return Returns a linear cell index.
	 */
	unsigned int SubscriptToIndex(unsigned int ncols, unsigned int row, unsigned int column);

		
};

#endif
