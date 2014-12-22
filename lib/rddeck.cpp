// rddeck.cpp
//
// Abstract a Rivendell Deck.
//
//   (C) Copyright 2002-2014 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <rddb.h>
#include <rdconf.h>
#include <rddeck.h>
#include <rdescape_string.h>

//
// Global Classes
//
RDDeck::RDDeck(QString station,unsigned channel,bool create)
{
  RDSqlQuery *q;
  QString sql;

  deck_station=station;
  deck_channel=channel;

  if(create) {
    sql=QString().sprintf("select ID from DECKS where \
(STATION_NAME=\"%s\")&&(CHANNEL=%d)",(const char *)deck_station,deck_channel);
    q=new RDSqlQuery(sql);
    if(q->size()!=1) {
      delete q;
      sql=QString().
        sprintf("INSERT INTO DECKS SET STATION_NAME=\"%s\",CHANNEL=%d",
		(const char *)deck_station,deck_channel);
      q=new RDSqlQuery(sql);
      delete q;
    }
    else {
      delete q;
    }
  }
}


QString RDDeck::station() const
{
  return deck_station;
}


int RDDeck::channel() const
{
  return deck_channel;
}


bool RDDeck::isActive() const
{
  return RDBool(GetStringValue("IS_ACTIVE"));
}


void RDDeck::setActive(bool state)
{
  SetRow("IS_ACTIVE",state);
}


bool RDDeck::monitorIsActive() const
{
  return RDBool(GetStringValue("MON_IS_ACTIVE"));
}


void RDDeck::setMonitorActive(bool state)
{
  SetRow("MON_IS_ACTIVE",state);
}


bool RDDeck::defaultMonitorOn() const
{
  return RDBool(GetStringValue("DEFAULT_MONITOR_ON"));
}


void RDDeck::setDefaultMonitorOn(bool state) const
{
  SetRow("DEFAULT_MONITOR_ON",state);
}


RDSettings::Format RDDeck::defaultFormat() const
{
  return (RDSettings::Format)GetIntValue("DEFAULT_FORMAT");
}


void RDDeck::setDefaultFormat(RDSettings::Format format) const
{
  SetRow("DEFAULT_FORMAT",(int)format);
}


int RDDeck::defaultChannels() const
{
  return GetIntValue("DEFAULT_CHANNELS");
}


void RDDeck::setDefaultChannels(int chan) const
{
  SetRow("DEFAULT_CHANNELS",chan);
}


int RDDeck::defaultBitrate() const
{
  return GetIntValue("DEFAULT_BITRATE");
}


void RDDeck::setDefaultBitrate(int rate) const
{
  SetRow("DEFAULT_BITRATE",rate);
}


int RDDeck::defaultThreshold() const
{
  return GetIntValue("DEFAULT_THRESHOLD");
}


void RDDeck::setDefaultThreshold(int level) const
{
  SetRow("DEFAULT_THRESHOLD",level);
}


QString RDDeck::switchStation() const
{
  return GetStringValue("SWITCH_STATION");
}


void RDDeck::setSwitchStation(QString str) const
{
  SetRow("SWITCH_STATION",str);
}


int RDDeck::switchMatrix() const
{
  return GetIntValue("SWITCH_MATRIX");
}


QString RDDeck::switchMatrixName() const
{
  QString matrix_name;

  RDSqlQuery *q=new RDSqlQuery(QString().sprintf("select NAME from MATRICES \
                             where (STATION_NAME=\"%s\")&&(MATRIX=%d)",
					       (const char *)switchStation(),
					       switchMatrix()));
  if(q->first()) {
    matrix_name=q->value(0).toString();
  }
  delete q;
  return matrix_name;
}


void RDDeck::setSwitchMatrix(int matrix) const
{
  SetRow("SWITCH_MATRIX",matrix);
}


int RDDeck::switchOutput() const
{
  return GetIntValue("SWITCH_OUTPUT");
}


QString RDDeck::switchOutputName() const
{
  QString output_name;

  RDSqlQuery *q=new RDSqlQuery(QString().sprintf("select NAME from OUTPUTS \
                             where (STATION_NAME=\"%s\")&&(MATRIX=%d)&&\
                                   (NUMBER=%d)",
					       (const char *)switchStation(),
					       switchMatrix(),
					       switchOutput()));
  if(q->first()) {
    output_name=q->value(0).toString();
  }
  delete q;
  return output_name;
}


void RDDeck::setSwitchOutput(int output) const
{
  SetRow("SWITCH_OUTPUT",output);
}


int RDDeck::switchDelay() const
{
  return GetIntValue("SWITCH_DELAY");
}


void RDDeck::setSwitchDelay(int delay) const
{
  SetRow("SWITCH_DELAY",delay);
}


int RDDeck::GetIntValue(const QString &field) const
{
  QString sql;
  RDSqlQuery *q;
  int accum;
  
  sql=QString().sprintf("select %s from DECKS where \
(STATION_NAME=\"%s\")&&(CHANNEL=%d)",(const char *)field,
			(const char *)deck_station,deck_channel);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    accum=q->value(0).toInt();
    delete q;
    return accum;
  }
  delete q;
  return 0;    
}


QString RDDeck::GetStringValue(const QString &field) const
{
  QString sql;
  RDSqlQuery *q;
  QString accum;
  
  sql=QString().sprintf("select %s from DECKS where \
(STATION_NAME=\"%s\")&&(CHANNEL=%d)",(const char *)field,
			(const char *)deck_station,deck_channel);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    accum=q->value(0).toString();
    delete q;
    return accum;
  }
  delete q;
  return 0;    
}


void RDDeck::SetRow(const QString &param,int value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE DECKS SET %s=%d \
WHERE (STATION_NAME=\"%s\")&&(CHANNEL=%d)",
			(const char *)param,
			value,
			(const char *)deck_station,
			deck_channel);
  q=new RDSqlQuery(sql);
  delete q;
}


void RDDeck::SetRow(const QString &param,const QString &value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE DECKS SET %s=\"%s\" \
WHERE (STATION_NAME=\"%s\")&&(CHANNEL=%d)",
			(const char *)param,
			(const char *)RDEscapeString(value),
			(const char *)deck_station,
			deck_channel);
  q=new RDSqlQuery(sql);
  delete q;
}


void RDDeck::SetRow(const QString &param,bool value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().sprintf("UPDATE DECKS SET %s=\"%s\" \
WHERE (STATION_NAME=\"%s\")&&(CHANNEL=%d)",
			(const char *)param,
			(const char *)RDYesNo(value),
			(const char *)deck_station,
			deck_channel);
  q=new RDSqlQuery(sql);
  delete q;
}
