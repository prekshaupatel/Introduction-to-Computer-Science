#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <vector>
#include <cstdio>
#include <ctime>
using namespace std;
const int binLen=8;
const int happycount=100;

//struct for node of binary tree
struct node
{
    string value;   //to place value of the phrase string
    vector<int> F;  //to keep track of all the positions
    node* left;
    node* right;
};

//creating a class for binary tree
class btree
{
private:
    node* root;
    void destroyNodes(node* start); //for recursive implementation of destructor
public:
    btree();
    ~btree();
    void insert(string key, int offset);  //to insert a new node
    node* search(string key);  //to find the string in the tree
    
};
btree::btree()
{
    root=nullptr;
}
//implement destructor since dynamic memory allocation occurs
btree::~btree()
{
    if(root!=nullptr)
        destroyNodes(root);
}
void btree::destroyNodes(node* start)  //helps in recursive implementation of the destructor
{
    if(start!=nullptr)
    {
        destroyNodes(start->left);
        destroyNodes(start->right);
    }
    delete start;
}

void btree::insert(string key, int offset)
{
    node* parent=root;  //helps keep track of the node at which the child pointer has to be changed
    node* start=root;
    
    while(start!=nullptr)
    {
        parent=start;
        if(start->value == key)
        {
            start->F.insert(std::upper_bound( start->F.begin(), start->F.end(), offset), offset); //insert offset in sorted vector at its correct position to keep the vector sorted
            return;
        }
        else if(start->value > key)
            start=start->left;
        else
            start=start->right;
    }
    node* n = new node;
    n->value = key;
    n->left=nullptr;
    n->right=nullptr;
    n->F.push_back(offset); //
    if(root==nullptr)
        root=n;
    else if(parent->value > key)
        parent->left=n;
    else
        parent->right=n;
}
node* btree::search(string key)
{
    node* start=root;
    while(start!=nullptr)
    {
        if(start->value==key)
            return start;
        else if(start->value > key)
            start=start->left;
        else
            start=start->right;
    }
    return nullptr;
}

//create a diff file
void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
    char charA[1];
    char charB[1];
    btree b;
    string strC="";
    char a;
    //create a binary tree with all possible sequences of 8 characters from the old file
    for(int count=0;count<binLen;count++)
    {
        fold.seekg(count, ios::beg);
        int i=0;
        strC="";
        while (fold.get(a))
        {
            i++;
            strC=strC+a;
            //if the string is 8 characters long add it to the tree
            if(i%binLen==0)
            {
                b.insert(strC,i-binLen+count);
                strC="";
            }
        }
        fold.clear();
    }
    
    bool bo=false;
    string p="";
    string add="";
    node* n;
    //for all the characters in the new file
    while(fnew.read (charA, 1))
    {
        p+=charA[0];
        //make p a string of 8 characters by adding characters till it is long enough
        while(p.length()<binLen && fnew.read (charA, 1))
        {
            p+=charA[0];
        }
        //search the tree for the 8 character sequence
        n=b.search(p);
        int start=0;
        int max=-1;
        int k=0;
        int st=-100;
        //if the sequence occurs in the binary tree
        if(n!=nullptr)
        {
            //for all the characters stored in the add string write a add instruction to the file
            if(add.length()!=0)
            {
                fdiff<<"A"<<add.length()<<":"<<add;
                add="";
            }
            int posQ=0;
            string strFind="";
            int counti=0;
            //read in the new few characters as per the value of happystring to compare with the following sequences from all the offsets in the old file
            //this helps reduce the time taken to read the string each time
            for(;fnew.read (charA, 1)&& counti<happycount;counti++)
                strFind+=charA[0];
            //while there are more offset positions, try to find a better one
            int loopcount=0;
            while(posQ < n->F.size() && max < counti && loopcount<500)
            {
                loopcount++;
                k=0;
                st=n->F[posQ];
                fold.clear();
                
                fold.seekg (st+binLen,ios::beg);
                fold.read (charB, 1);
                //k counts the number of matches following the matched sequence
                while(k<strFind.size() && strFind[k]==charB[0])
                {
                    k++;
                    if(!fold.read(charB, 1) )
                    {
                        fold.clear();
                        break;
                    }
                }
                
                //if there are more matches than the previous time, set it to the max value
                if(k>max)
                {
                    max=k;
                    start=st;
                }
                posQ++;
            }
            
            strFind="";
            fold.clear();
            //if the matches were equal to the maximum possible length of the string p, then look if there are possibly more matches
            if(max==happycount &&fold.seekg (st+max+binLen,ios::beg))
            {
                fold.read (charB, 1);
                while(charA[0]==charB[0])
                {
                    max++;
                    if(!fnew.read (charA, 1) || !fold.read(charB, 1) )
                        break;
                }
                //set the iterator of fnew to where it should continue from
                fnew.seekg (-1,ios::cur);
            }
            else
            {
                //set the iterator of fnew to where it should continue from
                if(counti<happycount)
                {
                    fnew.clear();
                    counti--;
                }
                fnew.seekg (-1-counti+max,ios::cur);
            }
            fdiff<<"C"<<binLen+max<<","<<start;
            //write the copy instruction
            bo=true;
            p="";
            
            //            cout<<fnew.tellg()<<endl;
        }
        else
        {
            //add the first character to the string and remove that character from the string p
            add+=p[0];
            p=p.substr(1);
            bo=false;
        }
        
        
    }
    //for the remaining characters, add them
    if(bo==true)
        add += p.substr(0,p.length()-1);
    else
        add+=p;
    if(add.length()!=0)
        fdiff<<"A"<<add.length()<<":"<<add;
    
}


bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
    char charA[1];
    char a;
    string p="";
    int i=0;
    int k=0;
    //for all the characters in the diff file
    while(fdiff.read (charA, 1))
    {
        a=charA[0];
        if(a=='A') //add instruction
        {
            fdiff>>i;
            fdiff.read (charA, 1);
            if(charA[0]!=':')  //check for correct format
                return false;
            for(int j=0; j<i-1;j++)  //read in the number of characters i
            {
                fdiff.read (charA, 1);
                fnew<<charA[0];
            }
            if(!fdiff.read (charA, 1))
                return false;
            fnew<<charA[0];
        }
        else if(a=='C') //copy instruction
        {
            fdiff>>i;
            fdiff.read (charA, 1);
            if(charA[0]!=',')  //check format
                return false;
            fdiff>>k;
            fold.clear();
            if(!fold.seekg(k,ios::beg))
                return false;
            for(int j=0; j<i-1;j++)   //read in stated characters from the old file
            {
                fold.read (charA, 1);
                fnew<<charA[0];
            }
            if(!fold.read (charA, 1))
                return false;
            fnew<<charA[0];
            
        }
        else if(a=='\r' ||a=='\n')  //do-nothing instruction
        {
            
        }
        else
            return false;
    }
    return true;
}






bool runtest(string oldName, string newName, string diffName, string newName2)
{
    if (diffName == oldName  ||  diffName == newName  ||
        newName2 == oldName  ||  newName2 == diffName  ||
        newName2 == newName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream diffFile(diffName, ios::binary);
    if (!diffFile)
    {
        cerr << "Cannot create " << diffName << endl;
        return false;
    }
    createDiff(oldFile, newFile, diffFile);
    diffFile.close();
    
    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the file
    ifstream diffFile2(diffName, ios::binary);
    if (!diffFile2)
    {
        cerr << "Cannot read the " << diffName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    assert(applyDiff(oldFile, diffFile2, newFile2));
    newFile2.close();
    
    newFile.clear();
    newFile.seekg(0);
    ifstream newFile3(newName2, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName2 << endl;
        return false;
    }
    istreambuf_iterator<char>begin1(newFile);
    istreambuf_iterator<char>end1;
    istreambuf_iterator<char>end2;
    istreambuf_iterator<char>begin2(newFile3);
    if (!(equal(begin1,end1,begin2)))
    {
        
        cerr << newName2 << " is not identical to " << newName
        << "; test FAILED" << endl;
        return false;
    }
    return true;
}

bool apply_diff(string oldName, string diffName, string newName2)
{
    if (diffName == oldName  || newName2 == oldName  ||  newName2 == diffName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }

    ifstream diffFile2(diffName, ios::binary);
    if (!diffFile2)
    {
        cerr << "Cannot read the " << diffName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    assert(applyDiff(oldFile, diffFile2, newFile2));
    newFile2.close();
    return true;
}




bool create_diff(string oldName, string newName, string diffName)
{
    if (diffName == oldName  ||  diffName == newName )
    {
        cerr << "Diff file must have a distinct name" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream diffFile(diffName, ios::binary);
    if (!diffFile)
    {
        cerr << "Cannot create " << diffName << endl;
        return false;
    }
    createDiff(oldFile, newFile, diffFile);
    diffFile.close();
    return true;
}


int main()
{
  int opt;
  cout<<"Please select one of the following options:\n1: Create a diff file\n2: Apply a diff file\n3: Check accuracy\n";
  cin>>opt;
  while(opt != 1 && opt != 2 && opt != 3)
    {
      cout<<"Please enter valid option (1/2/3)\n";
      cin.ignore(1000, '\n');
      cin>>opt;
    }
  cin.ignore(1000, '\n');
  string orig, modified, diff, remade;
  if (opt == 1)
    {
      cout<<"Enter absolute path for original file\n";
      getline(cin, orig);
      cout<<"Enter absolute path for modified file\n";
      getline(cin, modified);
      cout<<"Enter absolute path for diff file\n";
      getline(cin, diff);
      create_diff(orig, modified, diff);
    }
  else if (opt == 2)
    {
      cout<<"Enter absolute path for original file\n";
      getline(cin, orig);
      cout<<"Enter absolute path for diff file\n";
      getline(cin, diff);
      cout<<"Enter absolute path for new file\n";
      getline(cin, remade);
      apply_diff(orig, diff, remade);
    }
  else
    {
      cout<<"Enter absolute path for original file\n";
      getline(cin, orig);
      cout<<"Enter absolute path for modified file\n";
      getline(cin, modified);
      cout<<"Enter absolute path for diff file\n";
      getline(cin, diff);
      cout<<"Enter absolute path for new file\n";
      getline(cin, remade);
      assert(runtest(orig, modified, diff, remade));
    }
  return 0;
}
