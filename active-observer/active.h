class Active {
public:
  typedef function<void()> Message;
 
private:
 
  Active( const Active& );           // no copying
  void operator=( const Active& );    // no copying
 
  bool done;                         // le flag
  message_queue<Message> mq;        // le queue
  unique_ptr<thread> thd;          // le thread
 
  void Run() {
    while( !done ) {
      Message msg = mq.receive();
      msg();            // execute message
    } // note: last message sets done to true
  }
 
public:
 
  Active() : done(false) {
    thd = unique_ptr<thread>(
                  new thread( [=]{ this->Run(); } ) );
  }
 
  ~Active() {
    Send( [&]{ done = true; } ); ;
    thd->join();
  }
 
  void Send( Message m ) {
    mq.send( m );
  }
};
