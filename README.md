# License Plate Finder C++ (OpenCV) 
![](https://i.gyazo.com/2eb142300bd61df0fcfb6f2e33a7e3ee.png)  
# Steps
1. Find a clear image of a license plate
<p align="center">
<img src="https://i.gyazo.com/42cf2d2a26d9a07c7ef7aa17c4e0de9c.jpg" width="33%" height="33%"> 
</p>

2. Grayscale the image and perform both a bilateral filter and median blur  
**Both of these methods are effective for removing noise from our image while preserving edges**

<p align="center">
<img src="https://i.gyazo.com/02532756daf644c924600af1c5ec8565.jpg" width="33%" height="33%"> 
<img src="https://i.gyazo.com/7ab488eab89d35478400f66735475b9f.jpg" width="33%" height="33%"> 
</p>
 

3. Peform canny edge detection on our filtered image to find all the edges. 
<p align="center">
<img src="https://i.gyazo.com/fdf1b933f8ed0270640a9e96ae0e6fdd.jpg" width="33%" height="33%"> 
</p>

4. Use **findContours** to extract all the contours from our canny edged image.
<p align="center">
<img src="https://i.gyazo.com/2d221297d68eb72b9b07c7d3fc15a6a3.jpg" width="33%" height="33%"> 
</p>


5. Use **approxPolyDB** to approximate the polygonal curve of each contour, and only return the output vectors that have an exact size of four.
<p align="center">
<img src="https://i.gyazo.com/f42cfdc818583f3a215363050ac6fc42.jpg" width="33%" height="33%"> 
</p>

6. Now that we have all our contours with exactly four sides, we can find the one that contains the most contours inside it's bounds. In an application for *reading* license plates, 
this data would be processed using OCR. 
<p align="center">
<img src="https://i.gyazo.com/0bbaddd97c3d8894a100a2d18cb6afe8.jpg" width="50%" height="50%"> 
</p> 

7. Now, we can go ahead and draw that contour to our original image to outline the license plate. 
<p align="center">
<img src="https://i.gyazo.com/a777c840ccb163b33fa7d6d1a017a6b3.jpg" width="50%" height="50%"> 
</p> 


<img src="https://i.gyazo.com/ba44f91e24d97887491bae5f7078da32.png" >

