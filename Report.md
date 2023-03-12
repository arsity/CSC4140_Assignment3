# CSC4140 Assignment3

Haopeng Chen 120090645

## Task 1

First, use bounding box method to determine rasterization area. Then, use cross product to find the accurate position of each point (in/out) triangle. That is, the product of vectors from each point of the triangle to the target point with the vectors of triangles must be all positive or negative. At last, `rasterize_point` point by point.

<img src="./Report.assets/image-20230312073152594.png" alt="image-20230312073152594" style="zoom: 33%;" />

## Task 2

Supersampling is useful as it adopts convolution to pixel. On jaggies, convolution makes the edges become vague, leading to smoother lines. Instead of making adaptations everywhere, I basically implement the pipeline inside `rasterize_triangle`, making convolution to pixel points one by one in the bounding box intuitively.

Left to right: 1x, 4x, 16x (sample rates).

<img src="./Report.assets/image-20230312081558828.png" alt="image-20230312081558828" style="zoom:25%;" /><img src="./Report.assets/image-20230312081622151.png" alt="image-20230312081622151" style="zoom:25%;" /><img src="./Report.assets/image-20230312081650950.png" alt="image-20230312081650950" style="zoom:25%;" />

## Task 3

Do not familiar with front end things. No modification here.

<img src="./Report.assets/image-20230312083215422.png" alt="image-20230312083215422" style="zoom: 33%;" />

## Task 4

Barycentric coordinates could be considered as a transformation from 2D space (screen plane) into 3D space (RGB). The most important part is to use linear algebra knowledge to find the transformation matrix. 

From another perspective, it could also be considered as a 2D space (screen plane) mapping into 1D (line) problem. That is, we consider RGB as three independent channels and to find out their distribution separately.

<img src="./Report.assets/image-20230312083326292.png" alt="image-20230312083326292" style="zoom: 33%;" />

## Task 5

Pixel sampling is like color sampling, which is that the color is substitute by the pixel color after transformation of texture coordinate. 

<img src="./Report.assets/image-20230312084709785.png" alt="image-20230312084709785" style="zoom: 33%;" /><img src="./Report.assets/image-20230312090415044.png" alt="image-20230312090415044" style="zoom:33%;" /><img src="./Report.assets/image-20230312084725484.png" alt="image-20230312084725484" style="zoom: 33%;" /><img src="./Report.assets/image-20230312090448219.png" alt="image-20230312090448219" style="zoom:33%;" />
