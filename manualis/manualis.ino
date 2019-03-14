#include <UTFT2.h>
UTFT2 screen(ILI9341_16, 39, 41, 45, 47);

const int szelesseg = 5;
const int magassag = 5;

const int pixelmeret = 239 / max( szelesseg, magassag );


int meresek[magassag][szelesseg] = {0};

void setup() {

  screen.InitLCD();
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);

  Serial.begin(9600);

  negyzethalo();

  int legkisebb = 1024;
  int legnagyobb = 0;

  for ( int sor = 0; sor < magassag; sor++ )
  {
    Serial.print(sor + 1);
    Serial.println(". sor:");
    kirajzol_kozben( sor, 0 );
    delay(1000);
    for ( int oszlop = 0; oszlop < szelesseg; oszlop++ )
    {
      kirajzol_kozben( sor, oszlop );
      delay(1000);
      piros_negyzet( sor, oszlop );
      int ez = egymeres();
      meresek[sor][oszlop] = ez;
      if ( ez < legkisebb )
        legkisebb = ez;
      if ( legnagyobb < ez )
        legnagyobb = ez;
      Serial.print("Ez: "); Serial.println(ez);


    }
    Serial.println("===============================");
  }

  if ( legkisebb < legnagyobb )
  {
    negyzethalo();
    int hatar = ( legnagyobb + legkisebb ) / 2;

    Serial.print("Hatar: "); Serial.println(hatar);

    for ( int sor = 0; sor < magassag; sor++ )
    {
      for ( int oszlop = 0; oszlop < szelesseg; oszlop++ )
      {
        int ez = meresek[sor][oszlop];
        if ( ez < hatar )
        {
          Serial.print("X ");
          fekete_negyzet( sor, oszlop );
        }
        else
          Serial.print("  ");
      }
      Serial.println();
    }
  }
}


int egymeres()
{
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  delay(5);
  pinMode(A0, INPUT);
  analogRead(A0);
  delay(50);
  analogRead(A0);
  delay(1000);

  int atlag = 0;
  for ( int i = 0; i < 100; i++)
  {
    int mert = analogRead(A0);
    atlag += mert;
    Serial.print(i+1);Serial.print(";");Serial.println(mert);
    delay(10);
  }
  atlag /= 100;
  return atlag;
}


void negyzethalo()
{
  screen.fillScr(255, 255, 255);
  screen.setColor( 0, 0, 0 );
  for ( int i = 0; i <= magassag; i++)
    screen.drawLine( 0, i * pixelmeret, szelesseg * pixelmeret, i * pixelmeret );
  for ( int i = 0; i <= szelesseg; i++)
    screen.drawLine( i * pixelmeret, 0, i * pixelmeret, magassag * pixelmeret );
}

void piros_negyzet( int sor, int oszlop )
{
  screen.setColor( 255, 0, 0 );
  screen.fillRect( oszlop * pixelmeret + 1, sor * pixelmeret + 1, (oszlop + 1)*pixelmeret - 1, (sor + 1)*pixelmeret - 1 );
}
void sarga_negyzet( int sor, int oszlop )
{
  screen.setColor( 250, 240, 0 );
  screen.fillRect( oszlop * pixelmeret + 1, sor * pixelmeret + 1, (oszlop + 1)*pixelmeret - 1, (sor + 1)*pixelmeret - 1 );
}

void fekete_negyzet( int sor, int oszlop )
{
  screen.setColor( 0, 0, 0 );
  screen.fillRect( oszlop * pixelmeret + 1, sor * pixelmeret + 1, (oszlop + 1)*pixelmeret - 1, (sor + 1)*pixelmeret - 1 );
}


void kirajzol_kozben( int sor, int oszlop )
{
  negyzethalo();
  sarga_negyzet( sor, oszlop );
}





void loop()
{


}
