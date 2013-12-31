#include "csvfile.h"
#include "stringutils.h"

CsvFile::CsvFile()
{
}

CsvFile::CsvFile(const std::string& filename)
{
    loadFromFile(filename);
}

bool CsvFile::loadFromFile(const std::string& filename)
{
    bool status = false;
    lastFileRead = filename;
    std::vector<std::string> lines;
    if (StringUtils::readLinesFromFile(filename, lines))
    {
        for (const std::string& l: lines)
            parseLine(l);
        status = true;
    }
    return status;
}

void CsvFile::loadFromString(const std::string& str)
{
    std::vector<std::string> lines;
    StringUtils::getLinesFromString(str, lines);
    for (const std::string& l: lines)
            parseLine(l);
}

bool CsvFile::writeToFile(const std::string& newFilename) const
{
    std::string filename = newFilename;
    if (filename.empty())
        filename = lastFileRead;
    std::string str;
    writeToString(str);
    return StringUtils::writeStringToFile(filename, str);
}

void CsvFile::writeToString(std::string& str) const
{
    for (const Row& r: rows)
    {
        for (size_t i = 0; i < r.size(); ++i)
        {
            str += escapeQuotes(r[i]);
            if (i < r.size() - 1)
                str += ',';
        }
        str += '\n';
    }
}

CsvFile& CsvFile::clear()
{
    rows.clear();
    return *this;
}

CsvFile& CsvFile::addRow()
{
    rows.resize(rows.size() + 1);
    return *this;
}

CsvFile& CsvFile::addCell(const std::string& str)
{
    if (rows.empty())
        addRow();
    rows.back().push_back(str);
    return *this;
}

size_t CsvFile::numRows() const
{
    return rows.size();
}

CsvFile::Row& CsvFile::operator[](size_t i)
{
    return rows[i];
}

CsvFile::RowVector::iterator CsvFile::begin()
{
    return rows.begin();
}

CsvFile::RowVector::iterator CsvFile::end()
{
    return rows.end();
}

void CsvFile::parseLine(const std::string& line)
{
    enum State
    {
        Start = 0,
        Comma,
        Quote,
        End
    };

    //bool quote = false;
    bool inCell = false;

    addRow();

    std::string currentCell;

    // TODO: Handle quotes and stuff
    for (char c: line)
    {
        if (c == ',')
        {
            addCell(currentCell);
            currentCell.clear();
            inCell = false;
        }
        else
        {
            currentCell += c;
            inCell = true;
        }
    }

    if (inCell)
        addCell(currentCell);

    if (!line.empty() && line.back() == ',')
        addCell("");

    // Do this for now so it at least partially works
    //StringUtils::split(line, ",", rows.back());

    //std::cout << line << std::endl;
}

std::string CsvFile::escapeQuotes(std::string str) const
{
    // Escape any quotes, and if any were escaped, or if there are any commas, then add quotes
    if (StringUtils::replaceAll(str, "\"", "\"\"") || str.find(',') != std::string::npos)
        str = '"' + str + '"';
    return str;
}
