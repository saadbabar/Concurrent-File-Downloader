# Concurrent-File-Downloader  
**Concurrent File Downloader Personal Project**  

Making use of **C++**, **Multithreading**, **Docker**, and **Object-Oriented Programming Principles**.  

---

## Why This Project?  
This project dives deep into optimizing system performance for file downloads. For example, if your computer has an 8-core CPU, instead of using one core (thread) to download a file, this program allows you to break up the file and distribute each part to a thread, making use of maximum system resources to perform the download.  

The program will:  
1. Break the file into the specified number of threads.  
2. Distribute the work across the threads.  
3. Combine the outputs of each thread into one file, resulting in the final output.  

---

## How to Use  
The program takes in one optional argument and two positional arguments:  
1. **Optional Argument**: Specifies the number of threads you want to use (`-t <num>`). Pass this in first.  
2. **Positional Argument 1**: The file you want to download (a URL from online).  
3. **Positional Argument 2**: The output file name where you wish to store the contents.  

**Example**:  
```bash
./downloader -t 3 www.example.com/index.html output.txt
```

# Key Concepts Covered  

### **Object-Oriented Programming Concepts**  
- **Private and Public Member Functions/Variables**: Encapsulate data and methods within the `Downloader` class.  
- **Abstraction**: Hide complex details of how the program handles the file download. For example, calling the `Downloader` class in the main file only requires invoking a function without worrying about internal implementation.  
- **Encapsulation**: Bundle all variables and functions into a single unit (the `Downloader` class).  

### **Multithreading**  
- Use the `std::thread` library and mutexes for safe thread execution and concurrent downloading of file parts.  

### **File I/O**  
- Write into temporary files and combine them into the final output file (see the `download_chunk` function).  

### **HTTP/Networking Protocols**  
- Pull the HTTP response header of the URL to determine the file size and break the file into chunks for each thread to process.  

### **Docker**  
- Containerize the application for cross-platform compatibility after the initial coding is complete.  


## Metrics
- When given a 1.7GB video in a controlled environment (same WiFi, network speeds), this programw as able to utilize 8 threads on my M1 Macbook Air and download the video in 1 minute, 4 seconds. When Chrome's download manager was given the same video, it took 3 minutes and 23 seconds. Overall **1.95x** speed performance boost.
[!My Local Image](metric1.png)