/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef PARSER_NINETWO_H
#define PARSER_NINETWO_H

#include "parser_abstract.h"
#if defined(BUILD_FOR_QT5)
    #include <QUrlQuery>
#endif
#include <QNetworkReply>

// use native json if possible
#if defined(BUILD_FOR_QT5)
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#else
//otherwise load the workaround
#include "Qt4Json.h"
#endif

#include <QMap>

/**
 * @brief The ParserNinetwo class
 * Parser for the 9292ov.nl dutch public transport route planner backend.
 * it uses the unofficial json backend
 */
class ParserNinetwo : public ParserAbstract
{
    Q_OBJECT
    struct {
        QDateTime firstOption;
        QDateTime lastOption;
        Station from;
        Station via;
        Station to;
        int restrictions;
        Mode mode;

    } lastsearch;

    typedef enum restrictions{
        all=0,
        trainsOnly=1,
        noFerry=2
    } restrictions;

    int timetableRestrictions;

public:
    ParserNinetwo(QObject* parent = 0);

    // ParserAbstract interface
public:
    QString name(){return "9292ov.nl";}
    static QString getName() { return "9292ov.nl"; }
public slots:
    void getTimeTableForStation(const Station &currentStation, const Station &directionStation, const QDateTime &dateTtime, ParserAbstract::Mode mode, int trainrestrictions);
    void findStationsByName(const QString &stationName);
    void findStationsByCoordinates(qreal longitude, qreal latitude);
    void searchJourney(const Station &departureStation,const Station &viaStation,const Station &arrivalStation,const QDateTime &dateTime,const ParserAbstract::Mode mode, int trainrestrictions);
    void searchJourneyLater();
    void searchJourneyEarlier();
    void getJourneyDetails(const QString &id);
    bool supportsGps() { return true; }
    bool supportsVia() { return true; }
    bool supportsTimeTable() { return true; }
    bool supportsTimeTableDirection() { return false; }
    QStringList getTrainRestrictions();

protected:
    void parseTimeTable(QNetworkReply *networkReply);
    void parseStationsByName(QNetworkReply *networkReply);
    void parseStationsByCoordinates(QNetworkReply *networkReply);
    void parseSearchJourney(QNetworkReply *networkReply);
    void parseSearchLaterJourney(QNetworkReply *networkReply);
    void parseSearchEarlierJourney(QNetworkReply *networkReply);
    void parseJourneyDetails(QNetworkReply *networkReply);
    QMap<QString, JourneyDetailResultList*> cachedResults;
private:
    void parseJourneyOption(QJsonObject object);
};

#endif // PARSER_NINETWO_H