void C4040(){
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
}

void R4040(){
  digitalWrite(r4040, HIGH);
  digitalWrite(r4040, LOW);
}

void C4017(){
  digitalWrite(c4017, LOW);
  digitalWrite(c4017, HIGH);
  digitalWrite(c4017, LOW);
  
}

void R4017(){
  digitalWrite(r4017, HIGH);
  digitalWrite(r4017, LOW);
}

void RB(){            // Reset Both
  digitalWrite(r4040, HIGH);
  digitalWrite(r4040, LOW);
  
  digitalWrite(r4017, HIGH);
  digitalWrite(r4017, LOW);
}

void on(){            //Turns on display
  digitalWrite(DE, HIGH);
}

void off(){            //Turns off display
  digitalWrite(DE, LOW);
}

/*


void C511(){
  C256();
  C128();
  C64();
  C32();
  
  digitalWrite(c4040, HIGH); //24 counts  (16 + 8)
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  
  digitalWrite(c4040, HIGH);   //four counts
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  
  digitalWrite(c4040, HIGH);  // three counts (2 + 1)
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
}
  
  
  
  
  

void C256(){
  C64();
  C64();
  C64();
  C64();
}

void C128(){
  C64();
  C64();
}

void C64(){
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
}

void C32(){
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
  digitalWrite(c4040, HIGH);
  digitalWrite(c4040, LOW);
}



*/
