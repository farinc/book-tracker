# Book Tracker
## About
This project is part of a effort to help my mother in her small bookbinding business by providing her with a inventory management software that dealt with the organization of books and entries for her online store. 
## System Design Process
In order to design something that addressed her needs, I communicated with her regarding how books where managed so far and examining the _system_ that was already in place. Working with her system, we both came up with a implementation in the UI design and logic.
## Technologies
This project is written in C++ using Qt 5 toolkit for UI logic. The Qt 5 toolkit was choosen to maintain consistancy between different platforms as Linux, Mac, and Windows os's as all exist in our household. Book entries are stored as human editable/readable JSON files as they provide a cross-platform data serialization that of which wide support exists in various toolkits and languages. For JSON functionality, I used the JSON for Modern C++ library by Niels Lohmann as it provided a very simple implementation compared to a Qt solution. Since my mother currently uses a Windows computer, it was important that an simple installer was used, so I used CMake along with CPack + NSIS to compile and deploy the app. 
