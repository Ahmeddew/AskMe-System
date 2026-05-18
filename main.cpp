// Created by Ahmed Hany on 2026.
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

class User {
private:
    string userName;
    int user_id;
    string password;
    string email;
    bool AllowAnonymous;

public:
    User() : user_id(-1), AllowAnonymous(true) {}

    void setUserName(string Username) { this->userName = Username; }
    void setId(int id) { this->user_id = id; }
    void setpassword(string password) { this->password = password; }
    void setemail(string email) { this->email = email; }
    void setAnonymous_id(bool anonymous) { this->AllowAnonymous = anonymous; }

    string getUserName() { return userName; }
    string getPassword() { return password; }
    int getUserId() { return user_id; }
    string GetEmail() { return email; }
    bool AllowAnonymousId() { return AllowAnonymous; }
};

class Question {
private:
    int q_id;
    int parent_id;
    int from_user_id;
    int to_user_id;
    bool is_anonymous;
    string text;
    string answer;

public:
    Question() : q_id(-1), parent_id(-1), from_user_id(-1), to_user_id(-1), is_anonymous(false) {}

    bool isAnswered() { return !answer.empty(); }
    bool isThread() { return parent_id != -1; }

    void PrintQuestion() {
        cout << "Question Id (" << q_id << ")";
        if (!is_anonymous) {
            cout << " from user id (" << from_user_id << ")";
        } else {
            cout << " from [Anonymous]";
        }
        cout << "\t Question: " << text << endl;
        if (isAnswered()) {
            cout << "\t\t Answer: " << answer << endl;
        } else {
            cout << "\t\t Answer: Not Answered Yet" << endl;
        }
    }

    void setQuestionId(int id) { q_id = id; }
    void setParentId(int pid) { parent_id = pid; }
    void setFromUserId(int fid) { from_user_id = fid; }
    void setToUserId(int tid) { to_user_id = tid; }
    void setIsAnonymous(bool anon) { is_anonymous = anon; }
    void setText(string t) { text = t; }
    void setAnswer(string a) { answer = a; }

    int getQuestionId() { return q_id; }
    int getParentId() { return parent_id; }
    int getFromUserId() { return from_user_id; }
    int getToUserId() { return to_user_id; }
    bool getIsAnonymous() { return is_anonymous; }
    string getText() { return text; }
    string getAnswer() { return answer; }
};

class UserManger {
private:
    map<int, User> users_list;
    int last_id = 100;
public:
    User current_user;

    void appendToUsersFile(User &user) {
        ofstream fout("C:\\Users\\A store\\Desktop\\Backend practice\\AskMe\\users.txt", ios::app);
        if (fout.is_open()) {
            fout << user.getUserId() << " "
                 << user.getUserName() << " "
                 << user.getPassword() << " "
                 << user.GetEmail() << " "
                 << user.AllowAnonymousId() << endl;
            fout.close();
        } else {
            cout << "Critical Error: Could not save user data to file!" << endl;
        }
    }

    void LoadDatabase() {
        users_list.clear();
        ifstream fin("C:\\Users\\A store\\Desktop\\Backend practice\\AskMe\\users.txt");
        if (!fin.is_open()) return;

        int id;
        string name, email, password;
        bool anon;
        while (fin >> id >> name >> password >> email >> anon) {
            User u;
            u.setId(id);
            u.setUserName(name);
            u.setpassword(password);
            u.setemail(email);
            u.setAnonymous_id(anon);
            users_list[id] = u;
            if (id > last_id) { last_id = id; }
        }
        fin.close();
    }

    bool signup() {
        string username, password, email;
        bool AllowAnonymous;

        cout << "Enter UserName  : ";
        cin >> username;
        for (auto [id, user] : users_list) {
            if (user.getUserName() == username) {
                cout << "Error: Username already exists. Try again.\n";
                return false;
            }
        }
        cout << "Enter Password  : "; cin >> password;
        cout << "Enter Email     : "; cin >> email;
        cout << "Allow Anonymous questions ? (0 or 1) : "; cin >> AllowAnonymous;

        User newuser;
        last_id++;
        newuser.setAnonymous_id(AllowAnonymous);
        newuser.setemail(email);
        newuser.setpassword(password);
        newuser.setUserName(username);
        newuser.setId(last_id);

        users_list.insert({last_id, newuser});
        current_user = newuser;
        appendToUsersFile(newuser);
        cout << "Signup successful! Your ID is: " << last_id << endl;
        return true;
    }

    bool login() {
        LoadDatabase();
        string username, pass;
        cout << "Enter user name : "; cin >> username;
        cout << "Enter password  : "; cin >> pass;
        for (auto [id, user] : users_list) {
            if (user.getUserName() == username) {
                if (user.getPassword() == pass) {
                    cout << "You logged in successfully" << endl;
                    current_user = user;
                    return true;
                } else {
                    cout << "Invalid password. Try again." << endl;
                    return false;
                }
            }
        }
        cout << "Username not found." << endl;
        return false;
    }

    bool userExists(int id) {
        return users_list.find(id) != users_list.end();
    }

    User getUser(int id) {
        return users_list[id];
    }
};

class QuestionManger {
private:
    map<int, Question> Questions_list;
    int last_id;
    map<int, vector<int>> Question_threads;

    string replaceSpaces(string s) {
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == ' ') s[i] = '$';
        }
        return s;
    }
    string restoreSpaces(string s) {
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '$') s[i] = ' ';
        }
        return s;
    }

public:
    QuestionManger() { last_id = 2000; }

    void appendToQuestionsFile(Question Q) {
        ofstream fout("C:\\Users\\A store\\Desktop\\Backend practice\\AskMe\\questions.txt", ios::app);
        if (fout.is_open()) {
            string ans = Q.getAnswer();
            ans = (!ans.empty()) ? replaceSpaces(ans) : "Empty_Answer";
            fout << Q.getQuestionId() << " " << Q.getFromUserId() << " " << Q.getToUserId() << " "
                 << replaceSpaces(Q.getText()) << " " << ans << " " << Q.getIsAnonymous() << " " << Q.getParentId() << endl;
            fout.close();
        }
    }

    void LoadDatabase2() {
        Questions_list.clear();
        Question_threads.clear();
        ifstream fin("C:\\Users\\A store\\Desktop\\Backend practice\\AskMe\\questions.txt");
        if (!fin.is_open()) return;

        int qid, from, to, parentid;
        bool anon;
        string text, ans;
        while (fin >> qid >> from >> to >> text >> ans >> anon >> parentid) {
            Question Q;
            Q.setQuestionId(qid); Q.setFromUserId(from); Q.setToUserId(to);
            Q.setText(restoreSpaces(text));
            Q.setAnswer(ans == "Empty_Answer" ? "" : restoreSpaces(ans));
            Q.setIsAnonymous(anon); Q.setParentId(parentid);

            if (parentid != -1) {
                Question_threads[parentid].push_back(qid);
            }
            Questions_list[qid] = Q;
            if (qid > last_id) last_id = qid;
        }
        fin.close();
    }

    void UpdateDatabaseFile() {
        ofstream fout("C:\\Users\\A store\\Desktop\\Backend practice\\AskMe\\questions.txt", ios::trunc);
        if (fout.is_open()) {
            for (auto &[id, q] : Questions_list) {
                string ans = q.getAnswer();
                ans = (ans.empty()) ? "Empty_Answer" : replaceSpaces(ans);
                fout << q.getQuestionId() << " " << q.getFromUserId() << " " << q.getToUserId() << " "
                     << replaceSpaces(q.getText()) << " " << ans << " " << q.getIsAnonymous() << " " << q.getParentId() << endl;
            }
            fout.close();
        }
    }

    void AskQuestion(User &curr_user, UserManger &user_manager) {
        LoadDatabase2();
        int to_id, parent_id;
        cout << "Enter User ID to ask or -1 to cancel: "; cin >> to_id;
        if (to_id == -1) return;
        if (to_id == curr_user.getUserId()){
            cout << "Error: You cannot send a question to yourself!" << endl;
            return ;
        }

        if (!user_manager.userExists(to_id)) {
            cout << "User ID not found!\n";
            return;
        }

        User target_user = user_manager.getUser(to_id);
        cout << "Is it a thread reply? Enter Parent Question ID or -1 for a new question: "; cin >> parent_id;

        if (parent_id != -1 && Questions_list.find(parent_id) == Questions_list.end()) {
            cout << "Parent Question ID not found!\n";
            return;
        }

        bool is_anon = false;
        if (target_user.AllowAnonymousId()) {
            cout << "Send anonymously? (0 or 1): "; cin >> is_anon;
        } else {
            cout << "Note: Target user doesn't allow anonymous questions. Sent with your ID.\n";
        }
        cin.ignore();
        string text;
        cout << "Enter question text: ";
        getline(cin, text);

        last_id++;
        Question Q;
        Q.setQuestionId(last_id); Q.setFromUserId(curr_user.getUserId()); Q.setToUserId(to_id);
        Q.setText(text); Q.setIsAnonymous(is_anon); Q.setParentId(parent_id);

        Questions_list[last_id] = Q;
        if (parent_id != -1) Question_threads[parent_id].push_back(last_id);

        UpdateDatabaseFile();
        cout << "Question sent successfully! ID: " << last_id << endl;
    }

    void printFeed() {
        LoadDatabase2();
        for (auto &[id, question] : Questions_list) {
            if (question.getParentId() == -1) {
                cout << "\n------------------------------------------\n";
                question.PrintQuestion();
                if (Question_threads.count(id)) {
                    for (int childId : Question_threads[id]) {
                        cout << "\t Thread Reply => ";
                        Questions_list[childId].PrintQuestion();
                    }
                }
            }
        }
    }

    void AnswerQuestion(User &curr_user) {
        LoadDatabase2();
        int Qid;
        cout << "Enter the question id or -1 to cancel: "; cin >> Qid;
        if (Qid == -1) return;

        if (Questions_list.find(Qid) == Questions_list.end()) {
            cout << "Error: Question ID not found." << endl;
            return;
        }
        Question &q = Questions_list[Qid];
        if (q.getToUserId() != curr_user.getUserId()) {
            cout<<q.getToUserId()<<' '<<curr_user.getUserId()<<endl;
            cout << "Error: This question was not sent to you!" << endl;
            return;
        }
        q.PrintQuestion();
        cin.ignore();
        string newAnswer;
        cout << "Enter Answer: ";
        getline(cin, newAnswer);
        q.setAnswer(newAnswer);
        UpdateDatabaseFile();
        cout << "Answer saved successfully!" << endl;
    }

    void ListQuestionsToMe(User &curr) {
        LoadDatabase2();
        bool found = false;
        for (auto &[id, question] : Questions_list) {
            if (question.getToUserId() == curr.getUserId()) {
                if (question.getParentId() != -1) continue;
                cout << "Question Id (" << id << ") " << "from user " << question.getFromUserId() << "\n\tQuestion: " << question.getText() << endl;
                cout << "\tAnswer: " << question.getAnswer() << endl;
                if (Question_threads.count(id)) {
                    for (int childId : Question_threads[id]) {
                        cout << "\t thread : ";
                        Questions_list[childId].PrintQuestion();
                    }
                }
                cout << "  ----------------------------------- \n";
                found = true;
            }
        }
        if (!found) cout << "No Questions for you " << endl;
    }

    void ListQuestionsFromMe(User &curr) {
        LoadDatabase2();
        bool found = false;
        for (auto &[id, question] : Questions_list) {
            if (question.getFromUserId() == curr.getUserId()) {
                cout << "Question Id (" << id << ") to user " << question.getToUserId() << " [" << (question.getParentId() == -1 ? "Root" : "Reply") << "]\n";
                cout << "\t Question: " << question.getText() << endl;
                if (!question.getAnswer().empty()) cout << "\t Answer: " << question.getAnswer() << endl;
                else cout << "\t NOT Answered yet" << endl;
                cout << "  ----------------------------------- \n";
                found = true;
            }
        }
        if (!found) cout << "You haven't sent any Questions " << endl;
    }

    void DeleteQuestion(User &curr) {
        LoadDatabase2();
        cout << "Enter Question id or -1 to cancel: ";
        int q_id; cin >> q_id;
        if (q_id == -1) return;

        if (Questions_list.find(q_id) == Questions_list.end()) {
            cout << "Invalid ID!" << endl;
            return;
        }

        Question &q = Questions_list[q_id];
        if (q.getFromUserId() != curr.getUserId()) {
            cout << "You can only delete your own questions!" << endl;
            return;
        }

        if (Question_threads.count(q_id)) {
            for (int childId : Question_threads[q_id]) {
                Questions_list.erase(childId);
            }
            Question_threads.erase(q_id);
        }
        Questions_list.erase(q_id);
        UpdateDatabaseFile();
        cout << "Question and all associated threads deleted successfully." << endl;
    }
};

class AskMe {
private:
    QuestionManger questionManger; // Corrected variable name syntax
    UserManger userManger;

    void mainMenu() {
        while (true) {
            cout << "\nMenu : " << endl;
            cout << "\t1: Print Questions To me " << endl;
            cout << "\t2: Print Questions From me " << endl;
            cout << "\t3: Answer Question  " << endl;
            cout << "\t4: Delete Question  " << endl;
            cout << "\t5: Ask Question  " << endl;
            cout << "\t6: List System Feed  " << endl;
            cout << "\t7: Log out  " << endl;
            cout << "Enter choice (1-7): ";
            int choice; cin >> choice;

            switch (choice) {
                case 1:
                    questionManger.ListQuestionsToMe(userManger.current_user);
                    break;
                case 2:
                    questionManger.ListQuestionsFromMe(userManger.current_user);
                    break;
                case 3:
                    questionManger.AnswerQuestion(userManger.current_user);
                    break;
                case 4:
                    questionManger.DeleteQuestion(userManger.current_user);
                    break;
                case 5:
                    questionManger.AskQuestion(userManger.current_user, userManger);
                    break;
                case 6:
                    questionManger.printFeed();
                    break;
                case 7:
                    cout << "Logged out successfully.\n";
                    return;
                default:
                    cout << "Invalid choice! Enter a number between 1 and 7.\n";
                    break;
            }
        }
    }

public:
    void Run() {
        userManger.LoadDatabase(); // Optimized initialization location
        while (true) {
            cout << "\nMenu : " << endl;
            cout << "\t1: Login " << endl;
            cout << "\t2: Sign up " << endl;
            cout << "\t3: Exit System " << endl;
            cout << "Enter Number in Range 1 - 3: " << endl;
            int choice; cin >> choice;
            bool res;
            if (choice == 1) {
                res = userManger.login();
                if (res) {
                    mainMenu();
                }
            } else if (choice == 2) {
                res = userManger.signup();
                if (res) {
                    mainMenu();
                }
            } else if (choice == 3) {
                cout << "Thank you for using AskMe System. Goodbye!\n";
                break;
            } else {
                cout << "Invalid choice! Please enter a number between 1 and 3.\n";
            }
        }
    }
};

int main() {
    AskMe system;
    system.Run();
    return 0;
}