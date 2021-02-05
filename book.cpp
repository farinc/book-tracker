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

Book::Book(int bookID, int batchID): Book()
{
    this->batchID = batchID;
    this->bookID = bookID;
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

double Book::getCostByElement(std::string costType)
{
    return 0;
}

std::string Book::getName()
{
    return std::string("book-") + std::to_string(this->batchID) + std::to_string(this->bookID);
}

double Book::getExtraCosts()
{
    return this->costExtra + constants.pvaCost + constants.endpageCost;
}

double Book::getBoardCost()
{
    double paddedWidth = this->coverDim.width + constants.paddingWidthBoard;
    double paddedHeight = this->coverDim.height + constants.paddingHeightBoard;
    
    double sqInchBoard = paddedHeight * paddedWidth;
    return sqInchBoard * constants.sqInchBoardPrice;
}

double Book::getPageCost()
{
    int sheets = std::ceil(this->calculatePageCount() / 2);
    bool isHalfSheet = this->pageDim.width <= 4.25 || this->pageDim.height <= 5;
    double pricePages = sheets * constants.sheetPrice;
    
    if(isHalfSheet) {
        return pricePages / 2;
    }
    
    return pricePages;
}

double Book::getThreadRibbonCost()
{
    if(this->bookType != BookType::stabstich){
        double threadLength = (this->signitures * this->coverDim.height) + this->coverDim.height;
        double priceThread = threadLength * constants.threadLengthPrice;
        
        if(this->bookType == BookType::coptic2){
            priceThread *= 2;
        }
        
        return priceThread;
        
    }else{
        return this->coverDim.height * constants.ribbonPrice;
    }
}

double Book::getHeadbandCost()
{
    if(this->bookType == BookType::traditional || this->bookType == BookType::quater){
        return this->spine * 2 * constants.headbandPrice;
    }
    
    return 0;
}

double Book::getSuperCost()
{
    if(this->bookType == BookType::traditional or this->bookType == BookType::quater){
        double paddedSpine = this->spine + constants.paddingSpineForSuper;
        double sqInchSuper = paddedSpine * this->coverDim.height;
        return sqInchSuper * constants.superPrice;
    }
    
    return 0;
}

double Book::getClothCost()
{
    double paddedHeight = this->coverDim.height + constants.paddingHeightBoard;
    if(this->bookType == BookType::coptic || this->bookType == BookType::coptic2 || this->bookType == BookType::stabstich) {
        double paddedWidth = this->coverDim.width + constants.paddingWidthBoard;
        double sqInchCloth = paddedHeight * paddedWidth * 2;
        return sqInchCloth * constants.sqInchClothPrice;
    }else{
        double paddedSpine = this->spine;
        if(this->bookType == BookType::quater){
            paddedSpine += constants.paddingSpineQuarter;
        }else if(this->bookType == BookType::longstich || this->bookType == BookType::traditional){
            paddedSpine += constants.paddingSpineLongTrad;
        }
        
        double paddedWidth = this->coverDim.width + constants.paddingWidthBoard + paddedSpine;
        double sqInchCloth = paddedWidth * paddedHeight;
        return sqInchCloth * constants.sqInchClothPrice;
    }
    
    return 0;
}

Book Book::loadBook(const std::string path)
{
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    json jsonObj = json::parse(str);
    return jsonObj;
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
}




