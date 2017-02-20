# Gaussian_pyramid

The project is a small example of Gaussian pyramid and Luplcian pyramid.

There are few steps to create a gaussian pyramid
1. Weighted down using a Gaussian blur
2. Scale down with the length and width to 1/2.
3. repeat the 1&2 steps to make next layer of pyramid.

The result would like that 
https://github.com/night18/Gaussian_pyramid/blob/master/Gaussian.png

To make a Luplacian pyramid needs
1.Scale the layer up with the length and width to 2.
2.The previous Gaussian pyramid layer minus the scaled-up layer.
3. repeat the 1&2 steps to make next layer of pyramid.

The result would like that 
https://github.com/night18/Gaussian_pyramid/blob/master/Laplacian.png
