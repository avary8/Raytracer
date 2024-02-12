# Raytracer
In this project, I have included all library and header files needed inside lib and include for ease of linking. 

- A makefile is included to run by simply typing ```make``` or in my case, on my windows pc: ```mingw32-make```  
- a file called raytracing.exe should be created. To run this, type ```./raytracer```

  
If the makefile does not work another way is to type this compile command:  
```g++ main.cpp -o raytracer -L"./lib" -lopengl32 -lglew32 -lglfw3```  
- a file called raytracing.exe should be created. To run this, type ```./raytracer```

# Libraries 
Here is a simple guide to downloading the libraries (which is not needed since I have provided them; however, just as extra info) and c++ compiler. This guide is from the perspective of a Windows based setup and installation    
1. download msys https://www.msys2.org/    
        ucrt64 command line should open after installation  
2. enter the following 3 installations.   
    if you get error, go to the folder mysys2, scroll down to "ucrt64.exe" and run as admin
```pacman -S mingw-w64-ucrt-x86_64-gcc```
```pacman -S mingw-w64-ucrt-x86_64-glew```
```pacman -S mingw-w64-ucrt-x86_64-glfw```

3. ensure windows PATH environment points to the bin folder.    
    in my case: ```D:\Program Files\msys2\ucrt64\bin```
4. Download glm from : https://github.com/g-truc/glm or https://sourceforge.net/projects/glm.mirror/  

5. open vsc to project folder    
    type in terminal:   
    ```g++ *.cpp -o raytracer -L"<Lib File Location>" -lopengl32 -lglew32 -lglfw3```    
    so for example my command would look like this:    
    ```g++ *.cpp -o raytracer -L"D:\Program Files\msys32\ucrt64\lib" -lopengl32 -lglew32 -lglfw3```