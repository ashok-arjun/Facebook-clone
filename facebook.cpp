//Data Structures mini package to simulate the organization of users in facebook network
//Includes the implementation of various data structures including Graph, Trie, Queue and Stack

#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include"trie.h"
#include<fstream>
using namespace std;

struct TrieNode* rootOfTrie = getNode();

//doubly linked list of posts - specific to a user
struct Post{
    string author;
    string content;
    int likes;
    struct Post* previousPost;
    struct Post* nextPost;

};

//every newsfeed is user-specific
//it is a stack of posts LIFO
//every time a user posts something, the newsfeeds of all his/her friends get the particular post added to their stack

struct Feed{
    struct Post* post;
    struct Feed* next;
};


//queue of friend requests
struct FriendRequest{
    string nameOfUser;
    int idOfUser;
    struct FriendRequest* next;
};

//Graph
//every user is a node in the adjacency list
struct User{
    int user_id;
    string name;
    struct User* next; //refer graph1
    struct Post* posts; //denotes the head of posts - points to the first post
    struct FriendRequest* friendRequestsFront; //points to the first friend request
    struct FriendRequest* friendRequestsRear; //points to the last friend request
    struct Feed* feed;  //stack of posts: newsfeed
    bool active;
};


struct Graph{
    int V;
    struct User* users; //must be an array of adjacency lists, as the graph can be disconnected
};

typedef struct Graph* GRAPHPTR;

struct User* initializeFriend(int id,string name){
    struct User* temp = new User;
    temp->name = name;
    temp->user_id = id;
    temp->friendRequestsFront = temp->friendRequestsRear = NULL;
    temp->active = true;
    temp->next = NULL;
    temp->feed = NULL;
    temp->posts = NULL;

    return temp;
}

void addEdge(GRAPHPTR graph,int src,int destination){
    //append destination to src adj list
    struct User* destinationUser = initializeFriend(graph->users[destination].user_id,graph->users[destination].name);
    struct User* curUser = &graph->users[src];
    while(curUser->next != NULL){
        curUser = curUser->next;
    }
    curUser->next = destinationUser;

    //append src to destinations' adj list

    destinationUser = initializeFriend(graph->users[src].user_id,graph->users[src].name);
    curUser = &graph->users[destination];
    while(curUser->next != NULL){
        curUser = curUser->next;
    }
    curUser->next = destinationUser;
}

void initializeUser(struct User* user){
    user->name = "";
    user->next = NULL;
    user->posts = NULL;
    user->friendRequestsFront = NULL;
    user->friendRequestsRear = NULL;
    user->feed = NULL;
    user->active = false;
}



GRAPHPTR initializeGraph(int v){
    GRAPHPTR graph = new Graph;
    graph->V = v;
    graph->users = new User[v];
    for(int i = 0;i < v;i++){
        graph->users[i].user_id = i;
        initializeUser(&graph->users[i]);
    }

    return graph;
}


void addUser(GRAPHPTR graph,string name){
    transform(name.begin(),name.end(),name.begin(),::tolower);
    int i;
    for(i=0;i<graph->V;i++){
        if(graph->users[i].active == false){
            graph->users[i].name = name;
            graph->users[i].active = true;
            break;
        }
    }
    insert(rootOfTrie,name);
}


void displayAllUsersInTheSystem(GRAPHPTR graph){
    cout<<"All the users in the system : \n";

    for(int i=0;i<graph->V;i++){
        if(graph->users[i].active){
            cout<<graph->users[i].user_id<<" : "<<graph->users[i].name<<endl;
        }
    }
}


int displayParticularUser(GRAPHPTR graph,string name){

    for(int i=0;i<graph->V;i++){
        if(graph->users[i].name == name){
            cout<<graph->users[i].user_id<<" : "<<name<<endl;
        }
    }
    int id;
    cout<<"Enter the ID of the required person : \n";
    cin>>id;

    return id;
}

int getUserByID(GRAPHPTR graph){
    //assumed displayed

    //form trie
    //use autocomplete, retrieve user, send address

    //get atleast 3 characters
    int comp;
    int flag = 0;
    string reqName = "";
    char temp;
    cout<<"Enter the name of the person character by character: \n";
    /*while(reqName.length() < 2){
        cin>>temp;
        reqName.push_back(temp);
    }
    cout<<reqName<<endl;
    if(comp = printAutoSuggestions(rootOfTrie,reqName) == 0){
        cout<<"No matching strings !";
    }
    cout<<"\n\n\n"; */

    while(1){
        cout<<"Enter the next character!\n";
        cout<<reqName;
        cin>>temp;
        reqName.push_back(temp);

        cout<<"The name you have entered is : "<<reqName<<endl<<endl;

        if(comp = printAutoSuggestions(rootOfTrie,reqName) == 0){
            cout<<"No matching strings !\n";
            cout<<"Enter r to remove the previously entered character and continue, c to clear the search, x to abort search and exit \n";
            cin>>temp;
            if(temp == 'r'){
                reqName.pop_back();
            }
            else if(temp == 'c'){
                reqName.clear();
            }
            else if(temp == 'x'){
                return -1;
            }
        }
        else if(comp = printAutoSuggestions(rootOfTrie,reqName) == -1){
            cout<<endl<<"Is this the required name ? (y/n)";
            cin>>temp;
            if(temp == 'y' || temp == 'Y'){
                cout<<"Select the ID of the person : \n"; //as there can be more than one person with the same name, and in the trie it cannot recognize that
                return displayParticularUser(graph,reqName);
            }
        }
        cout<<endl;
    }



}



struct FriendRequest* getFriendRequest(){
    struct FriendRequest* fr = new FriendRequest;
    fr->next = NULL;
    return fr;
}

void enqueueFriendRequest(GRAPHPTR graph,int sourceUserID,int destinationUserID){
    struct FriendRequest* temp = getFriendRequest();
    temp->idOfUser = sourceUserID;
    temp->nameOfUser = graph->users[sourceUserID].name;
    //now coming to the enqueueing part
    if(graph->users[destinationUserID].friendRequestsRear == NULL){
        graph->users[destinationUserID].friendRequestsRear = graph->users[destinationUserID].friendRequestsFront = temp;
    }
    else{
        graph->users[destinationUserID].friendRequestsRear->next = temp;
        graph->users[destinationUserID].friendRequestsRear = temp;
    }
}

void dequeueFriendRequest(GRAPHPTR graph,int ID){
    struct FriendRequest* front1 = graph->users[ID].friendRequestsFront;

    if(front1->next == NULL){
        free(graph->users[ID].friendRequestsFront);
        graph->users[ID].friendRequestsFront = graph->users[ID].friendRequestsRear = NULL;
    }
    else{
        front1 = front1->next;
        free(graph->users[ID].friendRequestsFront);
        graph->users[ID].friendRequestsFront = front1;
    }
}

void displayFriendRequests(GRAPHPTR graph,int ID){
    struct FriendRequest* front1 = graph->users[ID].friendRequestsFront;
    struct FriendRequest* rear1 = graph->users[ID].friendRequestsRear;


    if(!front1 && !rear1){
        cout<<"No pending friend requests!"<<endl;
        return;
    }
    while(1){
        cout<<"Friend request received from : \n";
        cout<<"ID : "<<front1->idOfUser<<"  Name : "<<front1->nameOfUser<<endl;
        char choice;
        cout<<"To accept friend request, enter y, to reject it, enter n :\n\n";
        cin>>choice;

        if(choice == 'y'){
            addEdge(graph,ID,front1->idOfUser);
        }
        //now remove that friend request from queue, after storing the next request in front1
        if(front1 == rear1){
            dequeueFriendRequest(graph,ID);
            break;
        }
        front1 = front1->next;
        dequeueFriendRequest(graph,ID);
        if(front1 == NULL){
            break;
        }
    }

}

void sendFriendRequest(GRAPHPTR graph,int ID){
    User* user = &graph->users[ID];
    //view all users in the system, and using trie get the person's ID

    displayAllUsersInTheSystem(graph);
    cout<<"Enter the ID of the person whom you want to befriend : ";
    int destinationUserID;
    cin>>destinationUserID;
    if(destinationUserID > graph->V || destinationUserID < 0){
        return;
    }
    //trie can be used here to implement autocomplete
    enqueueFriendRequest(graph,ID,destinationUserID);
    cout<<"Friend request sent to "<<graph->users[destinationUserID].name<<endl;
}





void pushIntoFeedStack(GRAPHPTR graph,int ID,Post* srcPost){
    if(graph->users[ID].feed == NULL){
        graph->users[ID].feed = new Feed;
        graph->users[ID].feed->post = srcPost;
        graph->users[ID].feed->next = NULL;
    }
    else{
        Feed* tempFeed = new Feed;
        tempFeed->post = srcPost;
        tempFeed->next = graph->users[ID].feed;
        graph->users[ID].feed = tempFeed;
    }
}


void popFromFeedStack(GRAPHPTR graph,int ID){
    Feed* feedToBeDeleted = graph->users[ID].feed;
    graph->users[ID].feed = graph->users[ID].feed->next;
    delete feedToBeDeleted;
}


void addPost(GRAPHPTR graph,User* user){
    Post* post = new Post;
    post->previousPost = post->nextPost = NULL;
    post->author = user->name;
    cout<<"Enter the content : ";
    cin.ignore();
    getline(cin,post->content);
    post->likes = 0;
    //posts are doubly linked, so do the appropriate operation

    if(user->posts == NULL){
        user->posts = post;
    }
    else{
        Post* temp = user->posts;
        while(temp->nextPost != NULL){
            temp = temp->nextPost;
        }

        temp->nextPost = post;
        post->previousPost = temp;
    }

    //***************ALL THE ABOVE ARE USER SPECIFIC**********

    //***************NOW COMING TO GRAPH SPECIFIC**********

    User* friendOfUser = user->next;
    while(friendOfUser != NULL){
        pushIntoFeedStack(graph,friendOfUser->user_id,post);
        friendOfUser = friendOfUser->next;
    }

}

void viewPosts(User* user){
    Post* curPost = user->posts;
    while(1){
        if(curPost == NULL){
            return;
        }
        cout<<"\t"<<curPost->content<<"\n\tLikes : "<<curPost->likes;
        cout<<"\nEnter d to delete this post, n otherwise\n";
        char temp;
        cin>>temp;
        if(temp == 'd'){
            if(curPost->previousPost != NULL){
                Post* temp = curPost;
                curPost->previousPost->nextPost = curPost->nextPost;
                curPost = curPost->nextPost;
                free(temp);
            }
            else{
                user->posts = curPost->nextPost;
            }
        }
        if(curPost->previousPost){
            cout<<"\nEnter p to go to the previous post\n";
        }
        if(curPost->nextPost){
            cout<<"Enter n to go to the next post\n";
        }
        cout<<"Enter x to exit\n";

        cin>>temp;
        if(temp == 'p'){
            curPost = curPost->previousPost;
        }
        else if(temp == 'n'){
            curPost = curPost->nextPost;
        }
        else if(temp == 'x'){
            return;
        }
    }
}

void viewFeed(GRAPHPTR graph,int ID){

    Feed* currentFeed = graph->users[ID].feed; //actually the top pointer
    if(currentFeed == NULL){
        cout<<"There are no posts in you feed now!\nPlease come back later!\n";
        return;
    }

    //else, view the post of a feed with likes and have a like button, and increment likes, move to feed->next, and pop the previous feed
    while(currentFeed != NULL){
        cout<<"\t"<<currentFeed->post->author<<" : \n";
        cout<<"\t\t"<<currentFeed->post->content<<endl;
        cout<<"\t\tLikes : "<<currentFeed->post->likes<<endl<<endl;
        popFromFeedStack(graph,ID);
        cout<<"Enter l to like the post! Enter n to move to the next post! Enter x to stop viewing the feed\n";
        char c;
        cin>>c;
        if(c == 'l'){
            currentFeed->post->likes++;
            currentFeed = currentFeed->next;
        }
        else if(c == 'x'){
            return;
        }
        else if(c == 'n'){
            currentFeed = currentFeed->next;
        }

        if(currentFeed == NULL){
            return;
        }
    }

    cout<<"All the posts have been shown !\n";
}

void viewFriends(User* user){
    User* curFriend = user->next;
    while(curFriend){
        cout<<curFriend->name<<"\n";
        curFriend = curFriend->next;
    }
}


void friendSuggestions(GRAPHPTR graph,int ID){
    unordered_map<int,int> umap;
    User* user = &graph->users[ID];
    User* friendOfUser = user->next;
    while(friendOfUser){
        umap.insert(make_pair(friendOfUser->user_id,0));
        friendOfUser = friendOfUser->next;
    }
    friendOfUser = user->next;
    while(friendOfUser){
        User* searchUser = &graph->users[friendOfUser->user_id];
        User* friendOfSearchUser = searchUser->next;
        while(friendOfSearchUser){
            if(umap.find(friendOfSearchUser->user_id) != umap.end()){
                //implies that that id exists in the hash table
                umap[friendOfSearchUser->user_id]++;
            }

            friendOfSearchUser = friendOfSearchUser->next;
        }

        friendOfUser = friendOfUser->next;
    }

    //now find the keys with highest values, lower limit set to 3 mutual friends here
    cout<<"Friend suggestions, filtered with no. of mutual friends greater than or equal to 2:\n";
    for(auto i = umap.begin();i != umap.end();i++){
        //simple solution
        if(i->second >= 2){
            cout<<i->first<<" : "<<graph->users[i->first].name<<endl;
        }
    }

}


int main(){
    fstream file("names.txt",ios::in);
    GRAPHPTR graph = initializeGraph(111);
    //adding a node(user) into the database
    int i = 0;
    string tempName = "Arjun Ashok";
    while(file){
        i++;
        if(i == 31){
            break;
        }
        getline(file,tempName);
        addUser(graph,tempName);
    }
    //assume all the users have been added
    //interface

    int curID;
    int flag = 1;
    while(1){
        displayAllUsersInTheSystem(graph);
        cout<<"Enter your user ID : ";
        cin>>curID;
        flag = 1;
        struct User* currentUser = &graph->users[curID];
        int choice;
        while(1){
            if(flag == 0){
                break;
            }
            cout<<"1. View your feed\n2. Post\n3. View your posts\n4. View all your friend requests\n5. View all your friends\n6. Send friend requests\n7. View suggested friends\n8. Exit\nEnter your choice : ";
            cin>>choice;

            switch(choice){
                case 1:
                    viewFeed(graph,curID);
                    break;
                case 2:
                    addPost(graph,currentUser);
                    break;
                case 3:
                    viewPosts(currentUser);
                    break;
                case 4:
                    displayFriendRequests(graph,currentUser->user_id);
                    break;
                case 5:
                    viewFriends(currentUser);
                    break;
                case 6:
                    sendFriendRequest(graph,currentUser->user_id);
                    break;
                case 7:
                    friendSuggestions(graph,currentUser->user_id);
                    break;

                case 8:
                    cout<<"Thank you !\n";
                    flag = 0;
                    break;
                default:
                    cout<<"Invalid choice\n";
            }
        }
    }
    return 0;
}
