# Tree segmentation from a point cloud 

This is a basic implementation of the tree segmentation algorithm proposed in [1].

[1] W. Li, Q. Guo, M. K. Jakubowski, and M. Kelly, _A new method for segmenting individual trees from the lidar point cloud_, Photogrammetric Engineering and Remote Sensing, vol. 78, no. 1, pp. 75–84, 2012.

## Usage

TreeSegmentation "src_datasource_name" (e.g. TreeSegmentation my_file.csv)

## Description

The program takes a csv file containing a point cloud as input and outputs two .csv files containing the segmented points and derived tree attributes. The input csv should use a comma (,) as a separator without a header and have the following syntax:

x, y, z, classification
	 
Where:  
- x (double) : projected x-coordinate (no geographic coordinate)
- y (double) : projected y-coordinate (no geographic coordinate)
- z (double) : normalized point elevation
- classification (unsigned integer): unsigned integer representing the point classification


