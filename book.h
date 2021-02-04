#ifndef BOOK_H
#define BOOK_H

#include <json.hpp>
#include <QAbstractItemModel>
#include <ctime>

using json = nlohmann::json;

enum BookType: int {
    notype, traditional, coptic, coptic2, stabstich, quater, longstich
};

enum Status: int {
    nostatus, nophoto, draft, draftphoto, published, sold
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

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CostConstants, paddingWidthBoard, paddingHeightBoard, paddingSpineLongTrad, paddingSpineQuarter, paddingSpineForSuper, sqInchBoardPrice, sheetPrice, sqInchClothPrice, threadLengthPrice, headbandPrice, superPrice, ribbonPrice, pvaCost, endpageCost);
};

struct Dimension {
    float width;
    float height;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dimension, width, height);
};


class Book {

public:
    Book ();
    Book ( int bookID, int batchID, CostConstants constants );

    int bookID;
    int batchID;

    int signitures;
    int pagesPerSignitures;

    time_t lastEdit;
    time_t creation;

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
    float getCostByElement(const std::string);
    std::string getName();

    static Book loadBook(const std::string path);
    static void saveBook(Book book, const std::string path);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, bookID, batchID, signitures, pagesPerSignitures, lastEdit, creation, weight, spine, costExtra, box, section, threadColor, endpageColor, pageMaterial, coverMaterial, extra, coverDim, pageDim, status, bookType);

private:
    float getExtraCosts();
    float getBoardCost();
    float getPageCost();
    float getThreadRibbonCost();
    float getHeadbandCost();
    float getSuperCost();
    float getClothCost();
};

#endif // BOOK_H
