const int output_pin = 4;
const int test_pin = 5;
const int input_pin = 3;
const int list_length = 100;

const int lower_bound = 3;    //times between lower and upper bound will trigger the signal
const int upper_bound = 10;      
const int filter = 0;  //any time less than filter will not get recorded
const int threshold = list_length*0.4;

unsigned long start_time;
struct lList* first;
struct lList* last;

struct lList{
  unsigned long time;          //Need to find out the max size time can be so that I can min/max the size
  lList* next;
};

void setup() {
    //Serial.begin(9600);
    //while(!Serial){};


  pinMode(output_pin,OUTPUT);
  pinMode(test_pin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(input_pin),end,FALLING);
  attachInterrupt(digitalPinToInterrupt(input_pin-1),start,RISING);

  //TCCR1A = (1<<);
  //TCCR1A = 0; //clear presets

  TCCR1B  |= (1<<0);   //no prescaler

  TIMSK1 |= (1<<ICIE1);  //enable input capture
  TIFR1 |= (1<<ICF1);

  init_lList();



    //Serial.println("\n\n\nstarting...\n\n\n");

}


void loop() {
  int count = 0;
  
  noInterrupts();
  lList* current = first;
  if ((current->time > lower_bound)&&(current->time < upper_bound)){
    count++;
  }
  current = current->next;
  interrupts();

  while(current != NULL){
    if ((current->time > lower_bound)&&(current->time < upper_bound)){
      count++;
      //          digitalWrite(test_pin,1);
    }
    //else{
    //            digitalWrite(test_pin,0);
    //}
    current = current->next;
  }

  if(count > threshold)
    digitalWrite(output_pin,1);
  else
    digitalWrite(output_pin,0);

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
  TCNT1 = 0x0000;

  //Serial.println(TCNT1);
  //Serial.println("A");

  TIFR1=(1<<ICF1);
}

void end(){     //ends timer, adds value to linked list
  unsigned long end_time = TCNT1;

  //Serial.println(end_time);
  //Serial.println("B");

  if ((end_time) < filter){
    return;
  }

/*
    if ((end_time > lower_bound)&&(end_time < upper_bound)){
          digitalWrite(test_pin,1);
    }
    else{
          digitalWrite(test_pin,0);
    }
*/
  struct lList* temp = new lList;
  temp->next = NULL;
  
  temp->time = end_time;     //adds a new time to the end of the list
  last->next = temp;
  last = temp;

  temp = first;                     //deletes the beginning of the list
  first = temp->next;
  delete temp;
}