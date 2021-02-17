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

static std::string getString(BookType type)
{
    switch (type)
    {
    case 0: return "Not Specified";
    case 1: return "Traditional";
    case 2: return "Coptic";
    case 3: return "Two Needle Coptic";
    case 4: return "Stab Stich";
    case 5: return "Quarter";
    case 6: return "Long Stich";
    }
}
static std::string getString(Status type)
{
    switch (type)
    {
    case 0: return "Not Specified";
    case 1: return "No Photo";
    case 2: return "Drafted";
    case 3: return "Drafted w/ Photo";
    case 4: return "Published";
    case 5: return "Sold";
    }
}

static std::string getString(Dimension dim)
{
    return std::to_string(dim.width) + " in" + std::to_string(dim.height) + " in";
}

class Book {

public:
    Book ( int bookID = -1, int batchID = -1 );

    int bookID;
    int batchID;

    int signitures;
    int pagesPerSigniture;

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

    void updateTimestamp();

    bool isCalculatable() const;
    bool canHaveDiscription() const;
    int calculatePageCount() const;
    bool isValid() const;

    std::string getSpineType();
    double getExtraCosts() const;
    double getBoardCost() const;
    double getPageCost() const;
    double getThreadRibbonCost() const;
    double getHeadbandCost() const;
    double getSuperCost() const;
    double getClothCost() const;
    double getTotal() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, bookID, batchID, signitures, pagesPerSigniture, lastEdit, creation, weight, spine, costExtra, box, section, threadColor, endpageColor, pageMaterial, coverMaterial, extra, coverDim, pageDim, status, bookType);
};

inline bool operator==(Dimension dim1, Dimension dim2)
{
    bool flag = true;
    flag &= dim1.height == dim2.height;
    flag &= dim1.width == dim2.width;
    return flag;
}

inline bool operator==(Book book1, Book book2)
{
   bool flag = true;
   flag &= book1.batchID == book2.batchID;
   flag &= book1.bookID == book2.bookID;
   flag &= book1.signitures == book2.signitures;
   flag &= book1.pagesPerSigniture == book2.pagesPerSigniture;
   flag &= book1.bookType == book2.bookType;
   flag &= book1.status == book2.status;
   flag &= book1.weight == book2.weight;
   flag &= book1.costExtra == book2.costExtra;
   flag &= book1.spine == book2.spine;

   flag &= book1.box == book2.box;
   flag &= book1.coverMaterial == book2.coverMaterial;
   flag &= book1.pageMaterial == book2.pageMaterial;
   flag &= book1.endpageColor == book2.endpageColor;
   flag &= book1.threadColor == book2.threadColor;
   flag &= book1.section == book2.section;
   flag &= book1.extra == book2.extra;

   flag &= book1.coverDim == book2.coverDim;
   flag &= book1.pageDim == book2.pageDim;

   flag &= book1.creation == book2.creation; //The timestamp of creation is only used, an different edit time is excluded from equality
   return flag;
}

#endif // BOOK_H
