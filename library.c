#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// 函數宣告
void load_xml(const char *filename);
void add_book(const char *filename);
void modify_book(const char *filename);
void delete_book(const char *filename);
void query_books(const char *filename);
void list_books(const char *filename);

int main() {
    const char *filename = "books.xml";
    int choice;

    printf("=== 書籍管理系統 ===\n");
    printf("1. 新增書籍\n");
    printf("2. 修改書籍\n");
    printf("3. 刪除書籍\n");
    printf("4. 查詢書籍\n");
    printf("5. 列出所有書籍\n");
    printf("6. 結束系統\n");

    while (1) {
        printf("\n請選擇功能 (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_book(filename);
                break;
            case 2:
                modify_book(filename);
                break;
            case 3:
                delete_book(filename);
                break;
            case 4:
                query_books(filename);
                break;
            case 5:
                list_books(filename);
                break;
            case 6:
                printf("結束系統。\n");
                return 0;
            default:
                printf("無效選項，請重新選擇。\n");
        }
    }
}

void add_book(const char *filename) {
    char id[10], title[100], category[50], author[100], price[10];
    printf("請輸入書本編號: ");
    scanf("%s", id);
    printf("請輸入書本名稱: ");
    scanf("%s", title);
    printf("請輸入書類別: ");
    scanf("%s", category);
    printf("請輸入作者: ");
    scanf("%s", author);
    printf("請輸入價格: ");
    scanf("%s", price);

    // 載入 XML 文件
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        printf("無法讀取 XML 檔案。\n");
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr book = xmlNewChild(root, NULL, BAD_CAST "book", NULL);
    xmlNewProp(book, BAD_CAST "id", BAD_CAST id);
    xmlNewProp(book, BAD_CAST "category", BAD_CAST category);
    xmlNewChild(book, NULL, BAD_CAST "author", BAD_CAST author);
    xmlNewChild(book, NULL, BAD_CAST "title", BAD_CAST title);
    xmlNewChild(book, NULL, BAD_CAST "price", BAD_CAST price);

    // 儲存更新後的 XML
    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    xmlFreeDoc(doc);

    printf("書籍新增成功。\n");
}

void modify_book(const char *filename) {
    char id[10];
    printf("請輸入要修改的書本編號: ");
    scanf("%s", id);

    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        printf("無法讀取 XML 檔案。\n");
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr book = root->children;

    while (book) {
        if (xmlStrcmp(book->name, BAD_CAST "book") == 0) {
            xmlChar *book_id = xmlGetProp(book, BAD_CAST "id");
            if (xmlStrcmp(book_id, BAD_CAST id) == 0) {
                char title[100], category[50], author[100], price[10];
                printf("請輸入新書本名稱: ");
                scanf("%s", title);
                printf("請輸入新書類別: ");
                scanf("%s", category);
                printf("請輸入新作者: ");
                scanf("%s", author);
                printf("請輸入新價格: ");
                scanf("%s", price);

                xmlSetProp(book, BAD_CAST "category", BAD_CAST category);
                xmlNodePtr child = book->children;
                while (child) {
                    if (xmlStrcmp(child->name, BAD_CAST "author") == 0)
                        xmlNodeSetContent(child, BAD_CAST author);
                    else if (xmlStrcmp(child->name, BAD_CAST "title") == 0)
                        xmlNodeSetContent(child, BAD_CAST title);
                    else if (xmlStrcmp(child->name, BAD_CAST "price") == 0)
                        xmlNodeSetContent(child, BAD_CAST price);
                    child = child->next;
                }

                xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
                xmlFreeDoc(doc);
                printf("書籍修改成功。\n");
                return;
            }
            xmlFree(book_id);
        }
        book = book->next;
    }

    xmlFreeDoc(doc);
    printf("未找到書本編號 %s。\n", id);
}

void delete_book(const char *filename) {
    char id[10];
    printf("請輸入要刪除的書本編號: ");
    scanf("%s", id);

    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        printf("無法讀取 XML 檔案。\n");
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr book = root->children;

    while (book) {
        if (xmlStrcmp(book->name, BAD_CAST "book") == 0) {
            xmlChar *book_id = xmlGetProp(book, BAD_CAST "id");
            if (xmlStrcmp(book_id, BAD_CAST id) == 0) {
                xmlUnlinkNode(book);
                xmlFreeNode(book);
                xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
                xmlFreeDoc(doc);
                printf("書籍刪除成功。\n");
                return;
            }
            xmlFree(book_id);
        }
        book = book->next;
    }

    xmlFreeDoc(doc);
    printf("未找到書本編號 %s。\n", id);
}

void query_books(const char *filename) {
    char query[100];
    int found = 0;

    printf("請輸入查詢條件（例如書本編號、名稱、類別或作者）: ");
    scanf("%s", query);

    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        printf("無法讀取 XML 檔案。\n");
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr book = root->children;

    printf("=== 查詢結果 ===\n");
    while (book) {
        if (xmlStrcmp(book->name, BAD_CAST "book") == 0) {
            xmlChar *book_id = xmlGetProp(book, BAD_CAST "id");
            xmlChar *category = xmlGetProp(book, BAD_CAST "category");
            xmlNodePtr child = book->children;

            char author[100] = "", title[100] = "", price[10] = "";

            while (child) {
                if (xmlStrcmp(child->name, BAD_CAST "author") == 0)
                    strcpy(author, (const char *)xmlNodeGetContent(child));
                else if (xmlStrcmp(child->name, BAD_CAST "title") == 0)
                    strcpy(title, (const char *)xmlNodeGetContent(child));
                else if (xmlStrcmp(child->name, BAD_CAST "price") == 0)
                    strcpy(price, (const char *)xmlNodeGetContent(child));
                child = child->next;
            }

            if (strstr((const char *)book_id, query) ||
                strstr((const char *)category, query) ||
                strstr(author, query) ||
                strstr(title, query) ||
                strstr(price, query)) {
                printf("編號: %s, 類別: %s, 作者: %s, 書名: %s, 價格: %s\n",
                       book_id, category, author, title, price);
                found = 1;
            }

            xmlFree(book_id);
            xmlFree(category);
        }
        book = book->next;
    }

    if (!found) {
        printf("沒有找到符合條件的書籍。\n");
    }

    xmlFreeDoc(doc);
}

void list_books(const char *filename) {
    xmlDocPtr doc = xmlReadFile(filename, NULL, 0);
    if (!doc) {
        printf("無法讀取 XML 檔案。\n");
        return;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr book = root->children;

    printf("=== 書籍列表 ===\n");
    while (book) {
        if (xmlStrcmp(book->name, BAD_CAST "book") == 0) {
            xmlChar *book_id = xmlGetProp(book, BAD_CAST "id");
            xmlChar *category = xmlGetProp(book, BAD_CAST "category");
            xmlNodePtr child = book->children;

            char author[100] = "", title[100] = "", price[10] = "";

            while (child) {
                if (xmlStrcmp(child->name, BAD_CAST "author") == 0)
                    strcpy(author, (const char *)xmlNodeGetContent(child));
                else if (xmlStrcmp(child->name, BAD_CAST "title") == 0)
                    strcpy(title, (const char *)xmlNodeGetContent(child));
                else if (xmlStrcmp(child->name, BAD_CAST "price") == 0)
                    strcpy(price, (const char *)xmlNodeGetContent(child));
                child = child->next;
            }

            printf("編號: %s, 類別: %s, 作者: %s, 書名: %s, 價格: %s\n",
                   book_id, category, author, title, price);

            xmlFree(book_id);
            xmlFree(category);
        }
        book = book->next;
    }

    xmlFreeDoc(doc);
}
