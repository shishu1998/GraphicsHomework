picture = "P3 500 500 255 "
for y in range(1,500):
    for x in range(1,500):
        red = x % 255
        green = 0
        blue = y % 255
        picture += "%d %d %d "%(red,green,blue) 

        
f = open("image.ppm", "w")
f.write(picture)
f.close()
