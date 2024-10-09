
#include "Route.h"

Route::Route() {
  m_name = "Test";
  m_head = nullptr;
  m_tail = nullptr;
  m_size = 0;
}

Route::~Route() {
  Airport* current = m_head;
  //keep looping through the list until everything is deleted
  while (current != nullptr) {
    Airport* next = current->GetNext();
    delete current;
    current = next;
  }
  //make sure that the list is empty
  m_head = nullptr;
  m_tail = nullptr;
}

void Route::InsertEnd(string code, string name, string city, string country, double north, double west) {
  Airport* newAirport = new Airport(code, name, city, country, north, west);
  //if the list is empty the new airport becomes both heaed and tail
  if (m_head == nullptr) {
    m_head = newAirport;
    m_tail = newAirport;
  //if list is not empty then it becomes tail
  } else {
    m_tail->SetNext(newAirport);
    m_tail = newAirport;
  }
  m_size++;
  UpdateName();
}

void Route::RemoveAirport(int index) {
  if (index < 0 || index >= m_size || m_size <= 2) {
    cout << "Cannot remove airport at this index or route is too short." << endl;
    return;
      }

  Airport* current = m_head;
  Airport* previous = nullptr;
  //delet the airport based on what index the user selected
  //for removing the head
  if (index == 0) {
    m_head = current->GetNext();
    delete current;
  } else {
    //finding what airport to remove
    for (int i = 0; i < index; i++) {
      previous = current;
      current = current->GetNext();
    }
    previous->SetNext(current->GetNext());
    //for removing the tail
    if (index == m_size - 1) {
      m_tail = previous;
    }
    delete current;
  }
  m_size--;
  UpdateName();
}

void Route::SetName(string name) {
  m_name = name;
}

string Route::UpdateName() {
  if (m_head != nullptr && m_tail != nullptr) {
    m_name = m_head->GetCity() + " to " + m_tail->GetCity();
  }
  return m_name;
}

string Route::GetName() {
  return m_name;
}

int Route::GetSize() {
  return m_size;
}

void Route::ReverseRoute() {
  Airport* previous = nullptr;
  Airport* current = m_head;
    Airport* next = nullptr;
  m_tail = m_head;
  //this will iterate through the list of airport and reverse it
  while (current != nullptr) {
    next = current->GetNext();
    current->SetNext(previous);
    previous = current;
    current = next;
  }
  m_head = previous;
}

Airport* Route::GetData(int index) {
  if (index < 0 || index >= m_size) {
    return nullptr;
  }

  Airport* current = m_head;
  for (int i = 0; i < index; i++) {
    current = current->GetNext();
  }
  return current;
}

void Route::DisplayRoute() {
  Airport* current = m_head;
  int num = 1;
  //display the data of all of the airports
  while (current != nullptr) {
    cout << num << ". " << current->GetCode() << ", " << *current << ", " << current->GetCountry() << " (N" << current->GetNorth() << " W" << current->GetWest() << ")" << endl;
    current = current->GetNext();
    num++;
  }
}
