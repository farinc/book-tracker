#include "book.h"

#include <string>
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace books; 

Dimension::Dimension()
{
}

Dimension::Dimension (float width, float height)
{
    Dimension();
    this->width = width;
    this->height = height;
}

Dimension::~Dimension()
{
}


BookEntry::BookEntry(): coverDim(0, 0), pageDim(0, 0)
{
    this->bookID = -1;
    this->batchID = -1;
    this->signitures = 0;
    this->pagesPerSignitures = 0;

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
    
    this->status = Status::nostatus;
    this->bookType = BookType::notype;
}

BookEntry::BookEntry(int bookID, int batchID, CostConstants constants): BookEntry()
{
    this->batchID = batchID;
    this->bookID = bookID;
    this->constants = constants;
}

BookEntry::~BookEntry()
{}

bool BookEntry::isCalculatable()
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

int BookEntry::calculatePageCount()
{
    return this->signitures * this->pagesPerSignitures;
}

float BookEntry::getCostByElement(std::string costType)
{
    return 0;
}

float BookEntry::getExtraCosts()
{
    return this->costExtra + this->constants.pvaCost + this->constants.endpageCost;
}

float BookEntry::getBoardCost()
{
    float paddedWidth = this->coverDim.width + constants.paddingWidthBoard;
    float paddedHeight = this->coverDim.height + constants.paddingHeightBoard;
    
    float sqInchBoard = paddedHeight * paddedWidth;
    return sqInchBoard * this->constants.sqInchBoardPrice;
}

float BookEntry::getPageCost()
{
    int sheets = std::ceil(this->calculatePageCount() / 2);
    bool isHalfSheet = this->pageDim.width <= 4.25 || this->pageDim.height <= 5;
    float pricePages = sheets * this->constants.sheetPrice;
    
    if(isHalfSheet) {
        return pricePages / 2;
    }
    
    return pricePages;
}

float BookEntry::getThreadRibbonCost()
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

float BookEntry::getHeadbandCost()
{
    if(this->bookType == BookType::traditional || this->bookType == BookType::quater){
        return this->spine * 2 * this->constants.headbandPrice;
    }
    
    return 0;
}

float BookEntry::getSuperCost()
{
    if(this->bookType == BookType::traditional or this->bookType == BookType::quater){
        float paddedSpine = this->spine + this->constants.paddingSpineForSuper;
        float sqInchSuper = paddedSpine * this->coverDim.height;
        return sqInchSuper * this->constants.superPrice;
    }
    
    return 0;
}

float BookEntry::getClothCost()
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




