#ifndef BOOK_H
#define BOOK_H

#include <json.hpp>
#include <QAbstractItemModel>
#include <ctime>

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
    double paddingWidthBoard = 2.0;
    double paddingHeightBoard = 2.0;
    double paddingSpineLongTrad = 3.0;
    double paddingSpineQuarter = 5.0;
    double paddingSpineForSuper = 2.0;
    double sqInchBoardPrice = 0.02;
    double sheetPrice = 0.05;
    double sqInchClothPrice = 0.02;
    double threadLengthPrice = 0.002;
    double headbandPrice = 0.1;
    double superPrice = 0.02;
    double ribbonPrice = 0.10;
    double pvaCost = 0.5;
    double endpageCost = 0.5;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CostConstants, paddingWidthBoard, paddingHeightBoard, paddingSpineLongTrad, paddingSpineQuarter, paddingSpineForSuper, sqInchBoardPrice, sheetPrice, sqInchClothPrice, threadLengthPrice, headbandPrice, superPrice, ribbonPrice, pvaCost, endpageCost);
};

struct Dimension {
    double width;
    double height;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dimension, width, height);
};

class Book {

public:
    Book ( int bookID = -1, int batchID = -1 );

    int bookID;
    int batchID;

    int signitures;
    int pagesPerSignitures;

    time_t lastEdit;
    time_t creation;

    double weight;
    double spine;
    double costExtra;

    std::string box;
    std::string section;
    std::string threadColor;
    std::string endpageColor;
    std::string pageMaterial;
    std::string coverMaterial;
    std::string extra;

    Dimension coverDim;
    Dimension pageDim;

    Status status;
    BookType bookType;
    CostConstants constants;

    /**
     *  Determines if the book is at a minimum complete enough to get a price.
     **/
    bool isCalculatable() const;
    bool canHaveDiscription() const;
    int calculatePageCount() const;
    std::string getName() const;

    std::string getSpineType();
    double getExtraCosts() const;
    double getBoardCost() const;
    double getPageCost() const;
    double getThreadRibbonCost() const;
    double getHeadbandCost() const;
    double getSuperCost() const;
    double getClothCost() const;
    double getTotal() const;

    static Book loadBook(const std::string path);
    static void saveBook(Book book, const std::string path);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, bookID, batchID, signitures, pagesPerSignitures, lastEdit, creation, weight, spine, costExtra, box, section, threadColor, endpageColor, pageMaterial, coverMaterial, extra, coverDim, pageDim, status, bookType);
};

#endif // BOOK_H
