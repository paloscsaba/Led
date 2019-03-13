#include <UTFT2.h>
#include <AccelStepper.h>

const int szelesseg = 5;
const int magassag = 5;
const int pixelmeret = 239 / max( szelesseg, magassag );
int meresek[magassag][szelesseg] = {0};

UTFT2 screen(ILI9341_16, 39, 41, 45, 47);

AccelStepper xstepper( AccelStepper::FULL4WIRE, 63, 64, 65, 67);
AccelStepper ystepper( AccelStepper::FULL4WIRE, 4, 5, 6, 7);

bool xeleje()
{
  if ( !digitalRead(68) )
  {
    //Serial.println("xeleje!");
    return true;
  }
  return false;
}

bool xvege()
{
  if ( !digitalRead(69) )
  {
    //Serial.println("xvege!");
    return true;
  }
  return false;
}


bool yeleje()
{
  if ( !digitalRead(3) )
  {
    //Serial.println("yeleje!");
    return true;
  }
  return false;
}

bool yvege()
{
  if ( !digitalRead(2) )
  {
    //Serial.println("yvege!");
    return true;
  }
  return false;
}

void xvissza()
{
  Serial.println("xvissza");
  if ( xeleje() ) return;

  xstepper.move(-8000);
  while (xstepper.isRunning())
  {
    if ( xeleje() ) xstepper.stop();
    xstepper.run();
  }

  while ( xeleje() )
  {
    xstepper.move(300);
    while (xstepper.isRunning())
    {
      if ( !xeleje() ) xstepper.stop();
      xstepper.run();
    }
  }

}


void xlepes()
{
  Serial.println("xlepes");
  if ( xvege() ) return;

  xstepper.move(500);
  while (xstepper.isRunning())
  {
    if ( xvege() ) xstepper.stop();
    xstepper.run();
  }
}

void yvissza()
{
  Serial.println("yvissza");
  if ( yeleje() ) return;

  ystepper.move(-10000);
  while (ystepper.isRunning())
  {
    if ( yeleje() ) ystepper.stop();
    ystepper.run();
  }
}

void yvegig()
{
  Serial.println("yvegig");
  if ( yvege() ) return;

  ystepper.move(10000);
  while (ystepper.isRunning())
  {
    if ( yvege() ) ystepper.stop();
    ystepper.run();
  }
}

void ylepes()
{
  Serial.println("ylepes");
  if ( yvege() ) return;

  ystepper.move(500);
  while (ystepper.isRunning())
  {
    if ( yvege() ) ystepper.stop();
    ystepper.run();
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("itt vagyok");

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(68, INPUT);
  pinMode(69, INPUT);

  screen.InitLCD(PORTRAIT);
  negyzethalo();


  xstepper.setMaxSpeed(500.0);
  xstepper.setAcceleration(800);
  ystepper.setMaxSpeed(400.0);
  ystepper.setAcceleration(800);

  //  xvissza();
  //  yvissza();
  //  yvegig();

  int legkisebb = 1024;
  int legnagyobb = 0;

  //while (1)
  {
    yvissza();
    for ( int sor = 0; sor < magassag; sor++ )
    {
      ylepes();
      xvissza();

      Serial.println("=========================================");
      Serial.print(sor + 1);
      Serial.println(". sor:");
      kirajzol_kozben( sor, 0 );

      for ( int oszlop = 0; oszlop < szelesseg; oszlop++ )
      {
        kirajzol_kozben( sor, oszlop );
        xlepes();
        piros_negyzet( sor, oszlop );
        auto ez = egymeres();
        meresek[sor][oszlop] = ez;
        if ( ez < legkisebb )
          legkisebb = ez;
        if ( legnagyobb < ez )
          legnagyobb = ez;
        Serial.print("Ez: "); Serial.println(ez);
      }
    }

    if ( legkisebb < legnagyobb )
    {
      negyzethalo();
      auto hatar = ( legnagyobb + legkisebb ) / 2;

      Serial.print("Hatar: "); Serial.println(hatar);

      for ( int sor = 0; sor < magassag; sor++ )
      {
        for ( int oszlop = 0; oszlop < szelesseg; oszlop++ )
        {
          auto ez = meresek[sor][oszlop];
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

}


int egymeres()
{
  pinMode(A8, OUTPUT);
  digitalWrite(A8, LOW); // kinullazas
  delay(5);
  pinMode(A8, INPUT); // atvaltunk a meresre
  analogRead(A8);
  delay(5);
  analogRead(A8);
  delay(1000);        // varni kell, hogy stabilizalodjon a feszultseg

  int atlag = 0;
  const int mintaszam = 100;
  for ( int i = 0; i < mintaszam; i++)
  {
    int mert = analogRead(A8);
    atlag += mert;
    delay(10);
  }
  atlag /= mintaszam;
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
  // szuksegtelen
}







