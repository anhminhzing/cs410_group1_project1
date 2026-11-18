
#include <cassert>
#include <iostream>
#include "../include/Staff.h"

using namespace std;

int main() {
    Staff staff1("ST001", "Jordan Lee", "jordan@example.com",
                 "Facilities");

    assert(staff1.getUserID() == "ST001");
    assert(staff1.getName() == "Jordan Lee");
    assert(staff1.getEmail() == "jordan@example.com");
    assert(staff1.getDepartment() == "Facilities");
    assert(staff1.getUserType() == "Staff");

    staff1.displayStaff();

    cout << "-----------------------------------STAFF TESTS PASSED---------------------------------\n" << endl;

    return 0;
}