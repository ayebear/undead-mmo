#ifndef CSVFILE_H
#define CSVFILE_H

#include <string>
#include <vector>

/*
This class can be used to read and write CSV files.
*/
class CsvFile
{
    typedef std::vector<std::string> Row;
    typedef std::vector<Row> RowVector;

    public:
        CsvFile();
        CsvFile(const std::string&);
        bool loadFromFile(const std::string&);
        void loadFromString(const std::string&);
        bool writeToFile(const std::string& = "") const;
        void writeToString(std::string&) const;

        CsvFile& clear(); // Clears all of the rows
        CsvFile& addRow(); // Adds a new empty row
        CsvFile& addCell(const std::string&); // Adds another cell to the current row
        size_t numRows() const; // Returns the number of rows
        Row& operator[](size_t); // Returns a reference to a specific row
        RowVector::iterator begin();
        RowVector::iterator end();

    private:
        void parseLine(const std::string&);
        std::string escapeQuotes(std::string) const;

        std::string lastFileRead;
        RowVector rows;
};

#endif
