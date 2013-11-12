Preconditions:
- sqlite3 visible in system PATH

run:
bldmake bldfiles
abld export


if it is first time in platfrom run also 
X:\epoc32\rom\config>cone generate -c convert.confml -o \epoc32\rom\config
it is just to get it included in S60 root confml

Now you can start Carbide.Ct. Check videos about CT before.

Based on overrided configuration imaker will generated castoragedb in ROFS2/ROFS3
Check "Views" to change configuration.