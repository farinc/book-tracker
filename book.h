#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <json.hpp>

using json = nlohmann::json;

namespace books {

enum BookType: int {
    notype, traditional, coptic, coptic2, stabstich, quater, longstich
};

enum Status: int {
    nostatus, nophoto, draft, draftphoto, published, sold
};

struct CostConstants {
    // Padding Constants
    float paddingWidthBoard = 2.0F;
    float paddingHeightBoard = 2.0F;
    float paddingSpineLongTrad = 3.0F;
    float paddingSpineQuarter = 5.0F;
    float paddingSpineForSuper = 2.0F;

    // Pricing Constants
    float sqInchBoardPrice = 0.02F;
    float sheetPrice = 0.05F;
    float sqInchClothPrice = 0.02F;
    float threadLengthPrice = 0.002F;
    float headbandPrice = 0.1F;
    float superPrice = 0.02F;
    float ribbonPrice = 0.10F;
    float pvaCost = 0.5F;
    float endpageCost = 0.5F;
};

class Dimension {
    
public:
    Dimension();
    Dimension(float, float);
    ~Dimension();
    float width;
    float height;
};

class BookEntry {

public:
    BookEntry ();
    BookEntry ( int bookID, int batchID, CostConstants constants );
    ~BookEntry ();
    
    int bookID;
    int batchID;
    int signitures;
    int pagesPerSignitures;

    float weight;
    float spine;
    float costExtra;

    std::string box;
    std::string section;
    std::string threadColor;
    std::string endpageColor;
    std::string pageMaterial;
    std::string coverMaterial;
    std::string extra;

    Dimension coverDim;
    Dimension pageDim;
    CostConstants constants;

    Status status;
    BookType bookType;

    /**
     *  Determines if the book is at a minimum complete enough to get a price.
     **/
    bool isCalculatable();
    int calculatePageCount();
    float getCostByElement(std::string);

private:
    float getExtraCosts();
    float getBoardCost();
    float getPageCost();
    float getThreadRibbonCost();
    float getHeadbandCost();
    float getSuperCost();
    float getClothCost();
};

NLOHMANN_JSON_SERIALIZE_ENUM( BookType, {
    {notype, nullptr},
    {traditional, "traditional"},
    {coptic, "coptic"},
    {coptic2, "coptic2"},
    {stabstich, "stabstich"},
    {quater, "quater"},
    {longstich, "longstich"}
});
NLOHMANN_JSON_SERIALIZE_ENUM( Status, {
    {nostatus, nullptr},
    {nophoto, "nophoto"},
    {draft, "draft"},
    {draftphoto, "draftphoto"},
    {published, "published"},
    {sold, "sold"}
});
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CostConstants, paddingWidthBoard, paddingHeightBoard, paddingSpineLongTrad, paddingSpineQuarter, paddingSpineForSuper, sqInchBoardPrice, sheetPrice, sqInchClothPrice, threadLengthPrice, headbandPrice, superPrice, ribbonPrice, pvaCost, endpageCost);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Dimension, width, height);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BookEntry, bookID, batchID, signitures, pagesPerSignitures, weight, spine, costExtra, box, section, threadColor, endpageColor, pageMaterial, coverMaterial, extra, coverDim, pageDim, status, bookType);
}

#endif // BOOK_H
