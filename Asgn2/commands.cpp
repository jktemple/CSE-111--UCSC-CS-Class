// $Id: commands.cpp,v 1.27 2022-01-28 18:11:56-08 - - $

#include "commands.h"
#include "debug.h"

const command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result {cmd_hash.find (cmd)};
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such command");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status {exec::status()};
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}


void fn_cat (inode_state& state, const wordvec& words) {
   if(words.size() <= 1){
      return;
   }
   if(words[1][0]=='/' || words[1][0]=='.'){
      //parse pathname 
      wordvec s = split(words[1], "/");
      inode_ptr curr;
      cout<< s << endl;
      if(s.size() != 0) {
         if(words[1][0] == '/'){
            curr = state.get_root();
         }
         for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
         }
      } else {
         curr = state.get_root();
      }
      directory_entries entries = curr->get_dirents();
      for(directory_entries::iterator temp = entries.begin(); 
         temp != entries.end(); temp++){
            if(s[s.size()-1] == temp->first){
               cout<< temp->second->get_contents()->readfile() << endl;
               return;
            }
      }
      //cout<<"file does not exits"<<endl;
   } else {
      //make in current directory
      //filename is at words[1]
      //contents start at words[2]
      //cout<<"trying to make dir" << endl;
      wordvec s = split(words[1], "/");
      inode_ptr curr = state.get_cwd();
      for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
      }
      directory_entries entries = curr->get_dirents();
      for(directory_entries::iterator temp = entries.begin(); 
         temp != entries.end(); temp++){
            if(s[s.size()-1] == temp->first){
               cout<< temp->second->get_contents()->readfile() << endl;
               return;
            }
      }
      cout<<"file does not exits"<<endl;
   }
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() == 1 || words[1] == "/"){
      state.set_cwd(state.get_root());
   } else if(words[1] == ".") {
      return;
   } else {
      inode_ptr curr;
      if(words[1][0] == '/'){
         curr = state.get_root();
      } else {
         curr = state.get_cwd();
      }
      wordvec s = split(words[1], "/");
      if(s.size() != 0) {
         for(size_t i = 0; i < s.size(); i++) {
            curr = curr->get_dirents()[s[i]];
         }
      }
      state.set_cwd(curr);
   }
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   exec::status(stoi(words[1]));
   throw ysh_exit();
}

void printDirectory(inode_ptr node){
   try{
      node->get_dirents();
   } catch(file_error& e){
      return;
   }
   if(node == node->get_dirents()[".."]){
      cout << "/:" << endl;
   } else {
      wordvec printVec;
      inode_ptr parent = node->get_dirents()[".."];
      inode_ptr child = node;
      while(child != parent->get_dirents()[".."]){
         directory_entries entries = parent->get_dirents();
         for(directory_entries::iterator curr = entries.begin(); 
         curr != entries.end(); curr++){
            if(curr->second == child){
               printVec.push_back(curr->first);
               break;
            }
         }
         child = parent;
         parent = parent->get_dirents()[".."];
      }
      while(printVec.size() > 0){
         cout<< "/" << printVec.back();
         printVec.pop_back();
      }
      cout << "/:" << endl;
   }
   directory_entries entries = node->get_dirents();
   for(directory_entries::iterator curr = entries.begin(); 
      curr != entries.end(); curr++) {
         cout << setw(6) << curr->second->get_inode_nr() << "  " 
              << setw(6) << curr->second->get_contents()->size() << "  "
              << curr->first;
              try{
                 curr->second->get_dirents();
                 cout<<"/"<<endl;
              } catch(file_error& e) {
                 cout<<endl;
              }
   }
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   DEBUGS ('l', 
      const auto& dirents = state.get_root()->get_dirents();
      for (const auto& entry: dirents) {
         cerr << "\"" << entry.first << "\"->" << entry.second << endl;
      }
    );
    inode_ptr curr;
   directory_entries entries = state.get_cwd()->get_dirents();
   if(words.size() == 1 || words[1] == "."){
      entries = state.get_cwd()->get_dirents();
      curr = state.get_cwd();
   } else if(words[1] == "/") {
      entries = state.get_root()->get_dirents();
      curr = state.get_root();
   } else {
      wordvec s = split(words[1], "/");
      curr = state.get_root();
      if(s[0] == "."){
         curr = state.get_cwd();
      } 
      for(size_t i = 0; i < s.size(); i++) {
         curr = curr->get_dirents()[s[i]];
      }
   }
   printDirectory(curr);
}


void printDirectoryRecursive(inode_ptr node){
   directory_entries entries; 
   try{
      entries = node->get_dirents();
      printDirectory(node);
   } catch(file_error& e){
      return;
   }
   for(directory_entries::iterator curr = ++(++entries.begin()); 
   curr != entries.end(); curr++) {
      printDirectoryRecursive(curr->second);
   }
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr entries = state.get_cwd();
   if(words.size() == 1 || words[1] == "."){
      entries = state.get_cwd();
   } else if(words[1] == "/") {
      entries = state.get_root();
   } else {
      wordvec s = split(words[1], "/");
      inode_ptr curr = state.get_root();
      if(s[0] == "."){
         curr = state.get_cwd();
      } 
      for(size_t i = 0; i < s.size(); i++) {
         curr = curr->get_dirents()[s[i]];
      }
      entries = curr;
   }
   printDirectoryRecursive(entries);
}


void fn_make (inode_state& state, const wordvec& words) {
  if(words.size() <= 1){
      return;
   }
   if(words[1][0]=='/' || words[1][0]=='.'){
      //parse pathname 
      wordvec s = split(words[1], "/");
      inode_ptr curr;
      cout<< s << endl;
      if(s.size() != 0) {
         if(s[0] == "."){
            curr = state.get_cwd();
         } else {
            curr = state.get_root();
         }
         for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
         }
      } else {
         curr = state.get_root();
      }
      wordvec contents;
      for(size_t t = 2; t< words.size(); t++){
         contents.push_back(words[t]);
      }
      inode_ptr temp = curr->get_contents()->mkfile(s[s.size()-1]);
      temp->get_contents()->writefile(contents);
   } else {
      wordvec s = split(words[1], "/");
      inode_ptr curr = state.get_cwd();
      for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
      }
      wordvec contents;
      for(size_t t = 2; t< words.size(); t++){
         contents.push_back(words[t]);
      }
      inode_ptr temp = curr->get_contents()->mkfile(s[s.size()-1]);
      temp->get_contents()->writefile(contents);
      //cout << "made direc" << endl;
   }
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(words.size() <= 1){
      return;
   }
   if(words[1][0]=='/' || words[1][0]=='.'){
      //parse pathname 
      wordvec s = split(words[1], "/");
      inode_ptr curr;
      cout<< s << endl;
      if(s.size() != 0) {
         if(s[0] == "."){
            curr = state.get_cwd();
         } else {
            curr = state.get_root();
         }
         for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
         }
      } else {
         curr = state.get_root();
      }
      curr->get_contents()->mkdir(s[s.size()-1]);
   } else {
      //make in current directory
      //filename is at words[1]
      //contents start at words[2]
      //cout<<"trying to make dir" << endl;
      wordvec s = split(words[1], "/");
      inode_ptr curr = state.get_cwd();
      for(size_t i = 0; i < s.size()-1; i++) {
            curr = curr->get_dirents()[s[i]];
      }
      curr->get_contents()->mkdir(s[s.size()-1]);
      //cout << "made direc" << endl;
   }
}


void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   //cout << words << "\n just printed words";
   string newPrompt; 
   for(size_t i  = 1; i < words.size(); i++){
      newPrompt += words[i] + " ";
   }
   state.prompt(newPrompt + " ");
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   if(state.get_cwd() == state.get_root()){
      cout<< "/" << endl;
   } else {
      inode_ptr parent = state.get_cwd()->get_dirents()[".."];
      directory_entries entries = parent->get_dirents();
      for(directory_entries::iterator t = ++(++entries.begin()); 
      t != entries.end(); t++) {
         if(t->second == state.get_cwd()){
            cout<<"/" << t->first << endl;
            break;
         }
      }
   }
}

void fn_rm (inode_state& state, const wordvec& words) {
   inode_ptr curr;
   if(words[1][0]=='/'){
      curr = state.get_root();
   } else {
      curr = state.get_cwd(); 
   }
   wordvec s = split(words[1],"/");
   for(size_t i = 0; i < s.size() - 1; i++){
      curr = curr->get_dirents()[s[i]];
   }
   if(s.size()==0){
      return;
   }
   curr->get_contents()->remove(s[s.size()-1]);
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void recursiveDelete(inode_ptr node){
   //printDirectory(node);
   directory_entries entries;
   try{
      entries = node->get_dirents();
   } catch(file_error& e){
      return;
   }
   //cout<<"current recursive delete direc" << endl;
   for(directory_entries::iterator t = ++(++entries.begin()); 
      t != entries.end(); t++) { 
         recursiveDelete(t->second);
   }
   for(directory_entries::iterator t = ++(++entries.begin()); 
      t != entries.end(); t++) { 
         node->get_contents()->remove(t->first);
   }

}

void fn_rmr (inode_state& state, const wordvec& words) {
   inode_ptr curr;
   if(words[1][0]=='/'){
      curr = state.get_root();
   } else {
      curr = state.get_cwd(); 
   }
   wordvec s = split(words[1],"/");
   for(size_t i = 0; i < s.size() - 1; i++){
      curr = curr->get_dirents()[s[i]];
   }
   if(s.size()==0){
      return;
   }
   recursiveDelete(curr);
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

