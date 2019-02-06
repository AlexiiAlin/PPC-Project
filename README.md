# PPC-Project

Steps to run the project:

1. Download and install these 2 beauties : https://www.microsoft.com/en-us/download/details.aspx?id=57467

2. Run "set MSMPI" in cmd

    ![alt text](https://msdnshared.blob.core.windows.net/media/TNBlogsFS/prod.evol.blogs.technet.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/68/36/step2.png)

3. Install Visual Studio 2017 (Choose C++ environment)

4. Clone this project

5. Open the project with Visual Studio 2017

6. Right click on "PPC-Project" and click Properties

7. Under Configuration Properties -> C/C++ -> General -> [Additional Include Directories = $(MSMPI_INC);$(MSMPI_INC)\x64]

8. Under Configuration Properties -> Linker -> General -> [Additional Library Directories = $(MSMPI_LIB64)]

9. Apply & Ok, and you should be able to run the project.