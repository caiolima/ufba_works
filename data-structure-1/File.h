class File{

private:
  char *file_name;

public:
  File(char *);
  void init();
  bool write(int pos, user aUser);
  user get(int pos);

};
