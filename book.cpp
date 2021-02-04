#include "book.h"

#include <string>
#include <cmath>
#include <json.hpp>
#include <QAbstractItemModel>
#include <QUrl>
#include <QDebug>
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

Book::Book()
{
    this->coverDim = {0, 0};
    this->pageDim = {0,0};
    this->status = Status::nostatus;
    this->bookType = BookType::notype;
}

Book::Book(int bookID, int batchID, CostConstants constants): Book()
{
    this->batchID = batchID;
    this->bookID = bookID;
    this->constants = constants;
}

bool Book::isCalculatable()
{
    bool flag = true;
    flag &= this->coverDim.height > 0;
    flag &= this->coverDim.width > 0;
    flag &= this->pageDim.height > 0;
    flag &= this->pageDim.width > 0;
    flag &= this->spine > 0;
    flag &= this->calculatePageCount() > 0;
    flag &= this->bookType > 0;
    flag &= this->status > 0;

    return flag;
}

int Book::calculatePageCount()
{
    return this->signitures * this->pagesPerSignitures;
}

float Book::getCostByElement(std::string costType)
{
    return 0;
}

std::string Book::getName()
{
    return std::string("book-") + std::to_string(this->batchID) + std::to_string(this->bookID);
}

float Book::getExtraCosts()
{
    return this->costExtra + this->constants.pvaCost + this->constants.endpageCost;
}

float Book::getBoardCost()
{
    float paddedWidth = this->coverDim.width + constants.paddingWidthBoard;
    float paddedHeight = this->coverDim.height + constants.paddingHeightBoard;
    
    float sqInchBoard = paddedHeight * paddedWidth;
    return sqInchBoard * this->constants.sqInchBoardPrice;
}

float Book::getPageCost()
{
    int sheets = std::ceil(this->calculatePageCount() / 2);
    bool isHalfSheet = this->pageDim.width <= 4.25 || this->pageDim.height <= 5;
    float pricePages = sheets * this->constants.sheetPrice;
    
    if(isHalfSheet) {
        return pricePages / 2;
    }
    
    return pricePages;
}

float Book::getThreadRibbonCost()
{
    if(this->bookType != BookType::stabstich){
        float threadLength = (this->signitures * this->coverDim.height) + this->coverDim.height;
        float priceThread = threadLength * this->constants.threadLengthPrice;
        
        if(this->bookType == BookType::coptic2){
            priceThread *= 2;
        }
        
        return priceThread;
        
    }else{
        return this->coverDim.height * this->constants.ribbonPrice;
    }
}

float Book::getHeadbandCost()
{
    if(this->bookType == BookType::traditional || this->bookType == BookType::quater){
        return this->spine * 2 * this->constants.headbandPrice;
    }
    
    return 0;
}

float Book::getSuperCost()
{
    if(this->bookType == BookType::traditional or this->bookType == BookType::quater){
        float paddedSpine = this->spine + this->constants.paddingSpineForSuper;
        float sqInchSuper = paddedSpine * this->coverDim.height;
        return sqInchSuper * this->constants.superPrice;
    }
    
    return 0;
}

float Book::getClothCost()
{
    float paddedHeight = this->coverDim.height + this->constants.paddingHeightBoard;
    if(this->bookType == BookType::coptic || this->bookType == BookType::coptic2 || this->bookType == BookType::stabstich) {
        float paddedWidth = this->coverDim.width + this->constants.paddingWidthBoard;
        float sqInchCloth = paddedHeight * paddedWidth * 2;
        return sqInchCloth * this->constants.sqInchClothPrice;
    }else{
        float paddedSpine = this->spine;
        if(this->bookType == BookType::quater){
            paddedSpine += this->constants.paddingSpineQuarter;
        }else if(this->bookType == BookType::longstich || this->bookType == BookType::traditional){
            paddedSpine += this->constants.paddingSpineLongTrad;
        }
        
        float paddedWidth = this->coverDim.width + this->constants.paddingWidthBoard + paddedSpine;
        float sqInchCloth = paddedWidth * paddedHeight;
        return sqInchCloth * this->constants.sqInchClothPrice;
    }
    
    return 0;
}

Book Book::loadBook(const std::string path)
{
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    json jsonObj = json::parse(str);
    t.close();
    Book b = jsonObj;
    return b;
}

void Book::saveBook(Book book, const std::string path)
{
    std::ifstream w(path);
    time_t now;
    time(&now);

    book.lastEdit = now;
    if (!w)
    {
        book.creation = now; //if file did not exist, it is the first of its creation
    }
    std::ofstream t(path);
    json jsonObj = book;
    t << std::setw(4) << jsonObj << std::endl;
    t.close();
}




