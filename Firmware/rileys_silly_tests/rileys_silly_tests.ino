#include "./SD_test"
#include <stdio.h>
#include <unzipLIB.h>

class Book
{
  private:
    string title;
    string author;
    string publish_date;
    string added_date;
    int total_num_pages;

  public:
    int current_page;

}

int init_library()
{
  
  Book entry1;
  entry1.title = "abcd";
}
