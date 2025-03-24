#include "book.h"

#include <cmath>
#include <nlohmann/json.hpp>
#include <QDebug>

using json = nlohmann::json;

namespace bookdata {

Book::Book(int bookID)
{
    this->bookID = bookID;
    this->coverDim = {0, 0};
    this->pageDim = {0,0};
    this->status = Status::nostatus;
    this->bookType = BookType::notype;
    this->lastEdit = this->creation = time(0);
    this->signitures = 0;
    this->pagesPerSigniture = 0;
    this->weight = 0;
    this->spine = 0;
    this->costExtra = 0;
    this->box = std::string();
    this->section = std::string();
    this->threadColor = std::string();
    this->endpageColor = std::string();
    this->pageMaterial = std::string();
    this->coverMaterial = std::string();
    this->extra = std::string();
}

void Book::updateTimestamp(Book &book)
{
    book.lastEdit = time(0);
}

bool Book::isCalculatable(const Book &book)
{
    bool flag = true;
    flag &= book.coverDim.height > 0;
    flag &= book.coverDim.width > 0;
    flag &= book.pageDim.height > 0;
    flag &= book.pageDim.width > 0;
    flag &= book.spine > 0;
    flag &= calculatePageCount(book) > 0;
    flag &= book.bookType > 0;
    flag &= book.status > 0;
    return flag;
}

bool Book::canHaveDiscription(const Book &book)
{
    bool flag = true;
    flag &= !book.coverMaterial.empty();
    flag &= !book.threadColor.empty();
    flag &= !book.pageMaterial.empty();
    flag &= !book.endpageColor.empty();
    flag &= isCalculatable(book);
    return flag;
}

int Book::calculatePageCount(const Book &book)
{
    return book.signitures * book.pagesPerSigniture;
}

bool Book::isValid(const Book &book)
{
    return book.bookID >= 0;
}

std::string Book::getSpineType(const Book &book)
{
    std::string spineType;
    auto bookType = book.bookType;
    if(bookType == BookType::quater || bookType == BookType::longstich)
    {
        spineType = "Spine";
    }
    else if(bookType == BookType::coptic || bookType == BookType::coptic2)
    {
        spineType = "Thread";
    }
    else if(bookType == BookType::stabstich)
    {
        spineType = "Ribbon";
    }
    else
    {
        spineType = "";
    }

    return spineType;
}

double Book::getExtraCosts(const Book &book, const CostConstants &constants)
{
    return constants.pvaCost + constants.endpageCost;
}

double Book::getBoardCost(const Book &book, const CostConstants &constants)
{
    double paddedWidth = book.coverDim.width + constants.paddingWidthBoard;
    double paddedHeight = book.coverDim.height + constants.paddingHeightBoard;
    
    double sqInchBoard = paddedHeight * paddedWidth;
    return sqInchBoard * constants.sqInchBoardPrice;
}

double Book::getPageCost(const Book &book, const CostConstants &constants)
{
    int sheets = std::ceil(calculatePageCount(book) / 2);
    double pricePages = sheets * constants.sheetPrice;    
    return pricePages;
}

double Book::getThreadRibbonCost(const Book &book, const CostConstants &constants)
{
    if(book.bookType != BookType::stabstich){
        double threadLength = (book.signitures * book.coverDim.height) + book.coverDim.height;
        double priceThread = threadLength * constants.threadLengthPrice;
        
        if(book.bookType == BookType::coptic2){
            priceThread *= 2;
        }
        
        return priceThread;
        
    }else{
        return book.coverDim.height * constants.ribbonPrice * 5;
    }
}

double Book::getHeadbandCost(const Book &book, const CostConstants &constants)
{
    if(book.bookType == BookType::traditional || book.bookType == BookType::quater){
        return book.spine * 2 * constants.headbandPrice;
    }
    
    return 0;
}

double Book::getSuperCost(const Book &book, const CostConstants &constants)
{
    if(book.bookType == BookType::traditional || book.bookType == BookType::quater){
        double paddedSpine = book.spine + constants.paddingSpineForSuper;
        double sqInchSuper = paddedSpine * book.coverDim.height;
        return sqInchSuper * constants.superPrice;
    }
    
    return 0;
}

double Book::getClothCost(const Book &book, const CostConstants &constants)
{
    double paddedHeight = book.coverDim.height + constants.paddingHeightBoard;
    if(book.bookType == BookType::coptic || book.bookType == BookType::coptic2 || book.bookType == BookType::stabstich) {
        double paddedWidth = book.coverDim.width + constants.paddingWidthBoard;
        double sqInchCloth = paddedHeight * paddedWidth * 2;
        return sqInchCloth * constants.sqInchClothPrice;
    }else{
        double paddedSpine = book.spine;
        if(book.bookType == BookType::quater){
            paddedSpine += constants.paddingSpineQuarter;
        }else if(book.bookType == BookType::longstich || book.bookType == BookType::traditional){
            paddedSpine += constants.paddingSpineLongTrad;
        }
        
        double paddedWidth = book.coverDim.width + constants.paddingWidthBoard + paddedSpine;
        double sqInchCloth = paddedWidth * paddedHeight;
        return sqInchCloth * constants.sqInchClothPrice;
    }
    
    return 0;
}

double Book::getTotal(const Book &book, const CostConstants &constants)
{
    return getExtraCosts(book, constants) +
           getBoardCost(book, constants) +
           getPageCost(book, constants) +
           getThreadRibbonCost(book, constants) +
           getHeadbandCost(book, constants) +
           getSuperCost(book, constants) +
           getClothCost(book, constants) +
           book.costExtra;
}

std::string toString(Status status)
{
    switch (status)
    {
    case Status::draft:
        return "draft";
    case Status::draftphoto:
        return "draft w/ photo";
    case Status::nophoto:
        return "no photo";
    case Status::nostatus:
        return std::string();
    case Status::published:
        return "published";
    case Status::sold:
        return "sold";
    default:
        return std::string();
    }
}

};
