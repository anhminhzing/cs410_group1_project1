#include <iostream>
#include <cassert>
#include "../include/Admin.h"
using namespace std;

int main() {
    Admin admin("A001", "Alex", "alex@university.edu",
                "IT Office");

    // Test inherited User information
    assert(admin.getUserID() == "A001");
    assert(admin.getName() == "Alex");
    assert(admin.getEmail() == "alex@university.edu");

    // Test Admin-specific information
    assert(admin.getOffice() == "IT Office");
    assert(admin.getUserType() == "Admin");
    admin.displayAdmin();

    cout << "All Admin tests passed!" << endl;

    return 0;
}

