const int output_pin = 1;
const int input_pin = 2;
const int list_length = 300;

const int lower_bound = 100;    //times between lower and upper bound will trigger the signal
const int upper_bound = 70;      
const int filter = 10;  //any time less than filter will not get recorded
const int threshold = list_length*0.9;

unsigned long start_time;
struct lList* first;
struct lList* last;

struct lList{
  unsigned long time;          //Need to find out the max size time can be so that I can min/max the size
  lList* next;
};

void setup() {
  pinMode(output_pin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(input_pin),start,RISING);
  attachInterrupt(digitalPinToInterrupt(input_pin),end,FALLING);

  init_lList();
}


void loop() {
  int count = 0;

  noInterrupts();
  lList* current = first;
  if ((current->time > lower_bound)&&(current->time > upper_bound)){
    count++;
  }
  current = current->next;
  interrupts();

  while(current != NULL){
    if ((current->time > lower_bound)&&(current->time > upper_bound)){
      count++;
    }
    current = current->next;
  }
  //output_pin = count > threshold
}

void init_lList(){
  first = new lList;
  last = first;
  first->time = 0;
  for (int i=1;i<list_length;i++){
    struct lList* temp = new lList;
    temp->time = 0;
    last->next = temp;
    last = temp;
    last->next = NULL;
  }
}

void start(){   //starts timer
  start_time = micros();            //note I may redo this to directly work with the timer
}

void end(){     //ends timer, adds value to linked list
  int end_time = micros();

  if ((start_time - end_time) < filter){
    return;
  }

  struct lList* temp = new lList;
  temp->next = NULL;
  
  temp->time = start_time - end_time;     //adds a new time to the end of the list
  last->next = temp;
  last = temp;

  temp = first;                     //deletes the beginning of the list
  first = temp->next;
  delete temp;
}
