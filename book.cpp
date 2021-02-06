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

bool Book::isCalculatable() const
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

int Book::calculatePageCount() const
{
    return this->signitures * this->pagesPerSignitures;
}

std::string Book::getName() const
{
    return std::string("book-") + std::to_string(this->bookID);
}

double Book::getExtraCosts() const
{
    return this->costExtra + constants.pvaCost + constants.endpageCost;
}

double Book::getBoardCost() const
{
    double paddedWidth = this->coverDim.width + constants.paddingWidthBoard;
    double paddedHeight = this->coverDim.height + constants.paddingHeightBoard;
    
    double sqInchBoard = paddedHeight * paddedWidth;
    return sqInchBoard * constants.sqInchBoardPrice;
}

double Book::getPageCost() const
{
    int sheets = std::ceil(this->calculatePageCount() / 2);
    bool isHalfSheet = this->pageDim.width <= 4.25 || this->pageDim.height <= 5;
    double pricePages = sheets * constants.sheetPrice;
    
    if(isHalfSheet) {
        return pricePages / 2;
    }
    
    return pricePages;
}

double Book::getThreadRibbonCost() const
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

double Book::getHeadbandCost() const
{
    if(this->bookType == BookType::traditional || this->bookType == BookType::quater){
        return this->spine * 2 * constants.headbandPrice;
    }
    
    return 0;
}

double Book::getSuperCost() const
{
    if(this->bookType == BookType::traditional or this->bookType == BookType::quater){
        double paddedSpine = this->spine + constants.paddingSpineForSuper;
        double sqInchSuper = paddedSpine * this->coverDim.height;
        return sqInchSuper * constants.superPrice;
    }
    
    return 0;
}

double Book::getClothCost() const
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

double Book::getTotal() const
{
    return getExtraCosts() +  getBoardCost() + getPageCost() + getThreadRibbonCost() + getHeadbandCost() + getSuperCost() + getClothCost();}

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
        book.creation = now; //if file did not exist, we are creating a new one
    }
    std::ofstream t(path);
    json jsonObj = book;
    t << std::setw(4) << jsonObj << std::endl;
}




