#include "labellist.h"

#include <cassert>

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg) : msg_(msg) {
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken() : node_(nullptr), list_(nullptr) {}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
        // Add any initializers here
        : node_(node), list_(list) {}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const { return (this->node_ == rhs.node_); }

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const { return !operator==(rhs); }

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::operator bool() const { return this->isValid(); }

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const { return this->node_ != nullptr; }

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label) {
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index)) {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label) {
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index)) {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const {
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const {
    return this->isValid() && index != INVALID_LABEL && index < this->node_->labelled_.size()
           && this->node_->labelled_[index];
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType const& LabelList::MsgToken::msg() const {
    if (!(this->isValid())) {
        throw std::logic_error("Not a valid token");
    }
    return (this->node_->msg_);
}
// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType& LabelList::MsgToken::msg() {
    if (!(this->isValid())) {
        throw std::logic_error("Not a valid token");
    }
    return (this->node_->msg_);
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token) {
    if (token.isValid()) {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// To be completed - must adhere to any requirements in the .h file
LabelList::LabelList()
        // Add any initializers here
        : heads_(1), labels_(1), tail(nullptr) {
    labels_[0] = "all";
    heads_[0] = nullptr;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList() {clear(); }

// To be completed - must adhere to any requirements in the .h file
void LabelList::clear() {
    while (!empty()) {
        MsgToken m = MsgToken(heads_[0], this);
        this->remove(m);
    }
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::empty() const { return this->heads_[0] == nullptr; }

// Complete
size_t LabelList::size() const {
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while (n != nullptr) {
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::add(const MsgType& msg) {
    MsgNode* m1 = new MsgNode(msg);
    if (empty()) {
        this->heads_[0] = m1;
        this->tail = m1;
    } else {
        this->tail->next_[0] = m1;
        m1->prev_[0] = this->tail;
        this->tail = m1;
    }
    return MsgToken(m1, this);
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token) {
    if (!bool(token)) {
        return;
    }
    unsigned int max = token.node_->labelled_.size();

    if(token.node_->labelled_.size() > labels_.size()){
        max = labels_.size();
    }
    for (unsigned int i = 0; i < max; i++) {
        if (token.isLabelMember(i)) {
            this->unlabel(token, labels_[i]);
        }
    }
    if (heads_[0] == token.node_ && tail == token.node_) {
        heads_[0] = nullptr;
        tail = nullptr;
    } else if (heads_[0] == token.node_) {
        heads_[0] = token.node_->next_[0];
        heads_[0]->prev_[0] = nullptr;

    } else if (tail == token.node_) {
        tail = token.node_->prev_[0];
        tail->next_[0] = nullptr;

    } else {
        MsgToken before = token;
        MsgToken after = token;
        before.prev("all");
        after.next("all");
        before.node_->next_[0] = after.node_;
        after.node_->prev_[0] = before.node_;
    }
    token.node_->next_[0] = nullptr;
    token.node_->prev_[0] = nullptr;
    token.node_->labelled_[0] = false;

    delete token.node_;
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::label(const MsgToken& token, const LabelType& label) {
    if (!(token.isValid())) {
        throw std::invalid_argument("Bad token in label()");
    } else if(label != "all" &&  !(token.isLabelMember(label)) ){

        unsigned int label_index = findLabelIndex(label);

        if (findLabelIndex(label) == INVALID_LABEL) {  // if the label doesn't exist
            this->labels_.push_back(label);
            this->heads_.push_back(token.node_);
            label_index = labels_.size()-1;
        }

        for (unsigned int x = token.node_->labelled_.size() - 1; x < label_index - 1; x++) {  // push back null false null in the lists until you reach the right index
            token.node_->labelled_.push_back(false);
            token.node_->next_.push_back(nullptr);
            token.node_->prev_.push_back(nullptr);
        }
        token.node_->labelled_.push_back(true);  //push back null true null
        token.node_->next_.push_back(nullptr);
        token.node_->prev_.push_back(nullptr);

        if (heads_[label_index] != token.node_) {  // if token.node_  wasn't newly added to this label
            int i = label_index;
            MsgNode* tail_end;

            if (this->heads_[i] == nullptr) {  // if label exists but there is nothing else there (an empty label)
                token.node_->labelled_[i] = (true);
                token.node_->next_[i] = (nullptr);
                token.node_->prev_[i] = (nullptr);
                this->heads_[i] = (token.node_);
                tail_end = (token.node_);
                token.node_->labelled_[i] = (true);
            } else {  // there are members in this label
                int index = findToken(token, "all");
                tail_end = heads_[i];
                while(tail_end->next_[i] != nullptr){ //get the last item in this list
                    tail_end = tail_end->next_[i];         
                }

                if (index == 0) {  // if this is the first msg in the all list but not the label list you are adding to
                    token.node_->next_[i] = this->heads_[i];
                    this->heads_[i]->prev_[i] = token.node_;
                    this->heads_[i] = token.node_;
                } else if ((unsigned int)index == this->size() - 1) {  // if this msg is the last msg in the all list,
                                                                       // but not the label you're adding to
                    token.node_->prev_[i] = tail_end;
                    tail_end->next_[i] = token.node_;
                    tail_end = token.node_;
                } else {  // insert it into list
                    // start at our token in the all list and check for the next guy who is in the label we want
                    MsgToken point1 = token;
                    MsgToken point2 = token;
                    bool behind = false;
                    bool in_front = false;
                    while (point1.node_->prev_[0] != nullptr && !behind) {
                        point1.prev("all");
                        if (point1.isLabelMember(label)) {
                            behind = true;
                        }
                    }
                    while (point2.node_->next_[0] != nullptr && !in_front) {
                        point2.next("all");
                        if (point2.isLabelMember(label)) {
                            in_front = true;
                        }
                    }
                    if (behind) {
                        if (point1.node_->next_[i] != nullptr) {
                            token.node_->next_[i] = point1.node_->next_[i];
                            token.node_->prev_[i] = point1.node_;
                            point1.node_->next_[i] = token.node_;
                            token.node_->next_[i]->prev_[i] = token.node_;
                        } else {
                            point1.node_->next_[i] = token.node_;
                            token.node_->prev_[i] = point1.node_;
                            tail_end = token.node_;
                        }
                    } else if (in_front) {
                        if (point2.node_->prev_[i] != nullptr) {
                            token.node_->prev_[i] = point2.node_->prev_[i];
                            token.node_->next_[i] = point2.node_;
                            point2.node_->prev_[i] = token.node_;
                            token.node_->prev_[i]->next_[i] = token.node_;
                        } else {
                            point2.node_->prev_[i] = token.node_;
                            token.node_->next_[i] = point2.node_;
                            heads_[i] = token.node_;
                        }
                    }
                }

                token.node_->labelled_[i] = (true);
            }
        }
    }
}
// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label) {
    if (!(token.isValid())) {
        throw std::invalid_argument("Bad token in label()");
    }
    else if (label != "all") {
        size_t i = this->getLabelIndex(label); //see if this label actually exists
        if(token.isLabelMember(label)){ //if this token is apart of this label

            MsgNode* tail_end = heads_[i];

            while(tail_end->next_[i] != nullptr){ //get the last item in this list
                tail_end = tail_end->next_[i];         
            }

            if (heads_[i] == token.node_ && tail_end == token.node_) {
                heads_[i] = nullptr;
                tail_end = nullptr;
            } else if (heads_[i] == token.node_) {
                heads_[i] = token.node_->next_[i];
                token.node_->next_[i]->prev_[i] = nullptr;

            } else if (tail_end == token.node_) {  // if the last in the list
                tail_end = token.node_->prev_[i];
                tail_end->next_[i] = nullptr;
            } else {
                MsgToken before = token;
                MsgToken after = token;
                before.prev(label);
                after.next(label);
                before.node_->next_[i] = after.node_;
                after.node_->prev_[i] = before.node_;
            }
            token.node_->next_[i] = nullptr;
            token.node_->prev_[i] = nullptr;
            token.node_->labelled_[i] = false;
        
        

        
        }
        else{ // if not a part of this label
            return;
        }
    }
    
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label) {
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (n != nullptr) {
        if (n->msg_ == msg) {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label) {
    size_t loc = getLabelIndex(label);
    MsgNode* temp = this->heads_[loc];
    for (unsigned i = 0; i < index; i++) {
        if (temp == nullptr) {
            return end_;
        }
        temp = temp->next_[loc];
    }
    return MsgToken(temp, this);
}

// Complete - Do not alter
LabelList::MsgToken const& LabelList::end() const { return end_; }

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const {
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (nullptr != n) {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file
size_t LabelList::findLabelIndex(const LabelType& label) const {
    for (unsigned int i = 0; i < this->labels_.size(); ++i) {
        if (this->labels_[i] == label) {
            return i;
        }
    }
    return INVALID_LABEL;
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const {
    size_t retval = this->findLabelIndex(label);
    if (INVALID_LABEL == retval) {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}

int LabelList::findToken(const MsgToken& token, const LabelType& label) const { // checks the 0-based index of the msg in any label
    size_t index = getLabelIndex(label);
    MsgNode* n = this->heads_[index];
    int i = 0;
    while (n->next_[index] != nullptr) {
        if (n == token.node_) {
            return i;
        }
        i++;
        n = n->next_[index];
    }
    return i;
}
