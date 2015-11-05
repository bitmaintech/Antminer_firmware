BEGIN {
	FS =",";
 }

$1 !~ "^#" { system("${OE_BASE}/scripts/layerman " $1 " "  $2 " " $3 " " $4 " " command " " commandarg);}
