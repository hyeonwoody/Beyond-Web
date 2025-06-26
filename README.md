# Beyond-Web

</br>

## 🧑‍💻: Intro
> CLI-based program to enhance video editing convenience.

❓Problem : Needs to cut in time for editing video 😮

❗Idea : Organizing video footages in right order and right place consumes a lot of time 🤔

💯Solution : AUTOMATION 😁
 
 
</br>

 ## 🚀: Build & Run
0. Run ./app.sh & Run Beyond
    ```
    bash ./app.sh 
    ./build/Beyond -[Option Parameter] -[Flag Parameter]
    ```
    **If the script executes without errors, you can skip the following steps.**
 ---
1. Install ffmpeg(4.4.4 ~ 5.1.x):
    ```bash
    sudo apt install ffmpeg
    ```
2. Create a Build Directory:
    ```bash
    mkdir build
    cd build
    ```
 
3. Generate Build Files with CMake:
    ```bash
    cmake ..
    ```

4. Build the Project:
    ```bash
    make
    ```


5. Run the Excutable:
    ```bash
    ./Beyond -[Option Parameter] -[Flag Parameter]
    ```


 ## ✅: Implementation 
- **File copying**: Saving hard drive space by creating symbolic links in addition to copying the original file.  
- **Video cutting**: Cutting specific sections using the bookmark file of the player.  
- **Strategy pattern**: Adding a proceed() method to the CJob class to change the processing method at runtime.  
- **Command pattern**: Encapsulating the processing method in the CJob class to improve code readability.  
- **Factory method pattern**: Using the CreateOption() and CreateFlag() methods to create objects for optionList and flagList.  


  Options :  
    -w Set current path.  
    -c Set destination path.  
    -f File name alignment.  
    -~~s Select the file (Not Implemented).~~ 
  
  Flag :  
    -cp Copy the file to destination path(-c).  
    -sb Sync the subtitle file name with the video file name.  
    -sy Create Symbolic link to destination path(-c).  
    -ca Cut the video on certain duration.  
    
   
    
 </br>

## 📞: Contact
- Email: hyeonwoody@gmail.com
- Blog: https://velog.io/@hyeonwoody
- Github: https://github.com/hyeonwoody

</br>

## 🧱: Technologies Used
>C++  
>CMake

## 📖: Libraries Used
>FFmpeg (N-120025-gf67ca10f2e)  
