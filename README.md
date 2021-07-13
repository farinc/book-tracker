# Book Tracker
This project is part of a effort to help my mother in her small bookbinding business. This program was to provide her with a sort of inventory tracking that stores
book entries as human editable/readable JSON files. For JSON functionality, I used the JSON for Modern C++ library by Niels Lohmann and the many awsome contributes.
Without that library, this functionality would be a bit painful otherwise. The main goals was that it was simple and easy to use, along with being useful in terms of function. I also 
wanted it to be clear if changes where made and how they were to be saved (this functionality going through serveral revisions due to what I saw as useful to my
mother). Since she owns a Windows computer, it was important that an simple installer was used, so I used CMake + CPack + NSIS and a Windows Developement VM to 
compile and deploy the app. 
