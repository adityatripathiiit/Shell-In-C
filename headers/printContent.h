
// Print the contents of the directory
void printContent(struct dirent* name)
{
    // regular file type
    if(name->d_type == DT_REG) printf("%s%s    ",BLUE, name->d_name);

    // a directory type
    else if(name->d_type == DT_DIR) printf("%s%s/    ",GREEN, name->d_name);

    // unknown file type
    else printf("%s%s    ",BLUE, name->d_name );                             
}
