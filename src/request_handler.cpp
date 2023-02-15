#include "request_handler.h"
#include <iomanip>
#include <boost/mysql.hpp>
#include "http_status.h"
#include "logger.h"

using namespace boost;
using namespace logs;

request_handler::request_handler(io_context &context)
    : context_(context)
{
}

void request_handler::init_db(char *host_ip, std::size_t host_port, char *db_name, string &user_name, string &user_password)
{
  LOG(logINFO) << "init DB begin";
  database_ = db_name;
  db_host_port = host_port;
  db_user_name = user_name;
  db_password = user_password;
  db_host = host_ip;

  ip::tcp::endpoint ep(
        ip::address::from_string(db_host),
        db_host_port
        );

  mysql::handshake_params params(
        db_user_name,
        db_password,
        "", // database
        mysql::collation::utf8_general_ci,
        mysql::ssl_mode::disable);

  mysql::tcp_connection mysql_conn(context_);
  mysql_conn.connect(ep, params);

  string sql;
  mysql::resultset result;

  mysql_conn.query("CREATE DATABASE IF NOT EXISTS " + database_, result);
  mysql_conn.query("USE " + database_, result);
  mysql_conn.query("DROP TABLE IF EXISTS gambling_books", result);
  sql =
      "CREATE TABLE gambling_books ("
      "gambling_book_id int unsigned NOT NULL AUTO_INCREMENT,"
      "short_name varchar(30) NOT NULL DEFAULT '',"
      "gambling_book varchar(255) NOT NULL DEFAULT '',"
      "author varchar(255) NOT NULL DEFAULT '',"
      "is_goodreads_author enum('Y','N') NOT NULL DEFAULT 'N',"
      "rating_avg decimal(6,2) NOT NULL DEFAULT '0.00',"
      "ratings int NOT NULL DEFAULT '0',"
      "score int NOT NULL DEFAULT '0',"
      "person_voted int NOT NULL DEFAULT '0',"
      "price decimal(15,2) NOT NULL DEFAULT '0.00',"
      "year smallint NOT NULL DEFAULT '0',"
      "PRIMARY KEY (gambling_book_id)"
      ") ENGINE=MyISAM DEFAULT CHARSET=utf8";
  mysql_conn.query(sql, result);

  sql = "INSERT INTO gambling_books VALUES (1,'The Dark Side of the Felt','The Dark Side of the Felt','Tyler Nals','Y',3.78,114,900,9,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (2,'All On Zero','All On Zero','Argiro Mantoglou','Y',4.35,17,300,3,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (3,'Craps: The Ultimate Winning St','Craps: The Ultimate Winning Strategies','Will Thomas','N',3.67,3,200,2,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (4,'Gambling 102: The Best Strateg','Gambling 102: The Best Strategies for All Casino Games','Mike Shackleford','N',3.92,26,196,2,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (5,'Burning the Tables in Las Vega','Burning the Tables in Las Vegas: Keys to Success in Blackjack and in Life','Ian Andersen','N',3.93,60,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (6,'Comped','Comped','Bill Kearney','N',4.15,20,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (7,'Learn Pro Blackjack','Learn Pro Blackjack','Simplicent LLC','N',5,1,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (8,'Blood Aces: The Wild Ride of B','Blood Aces: The Wild Ride of Benny Binion, the Texas Gangster Who Created Vegas Poker','Doug J. Swanson','Y',3.86,670,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (9,'Roulette Raider','Roulette Raider','Jake Castle','N',4,2,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (10,'The Puzzlewright Guide to Casi','The Puzzlewright Guide to Casino Gambling','Andrew Brisman','N',0,0,100,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (11,'Video Poker for the Intelligen','Video Poker for the Intelligent Beginner','Bob Dancer','N',3.56,36,99,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (12,'The Kama Sutra of Poker','The Kama Sutra of Poker','Victor Irons','N',5,2,99,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (13,'Blackbelt in Blackjack: Playin','Blackbelt in Blackjack: Playing Blackjack as a Martial Art','Arnold Snyder','Y',4.15,55,98,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (14,'Beat the Dealer: A Winning Str','Beat the Dealer: A Winning Strategy for the Game of Twenty-One','Edward O. Thorp','N',4.17,710,97,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (15,'Gamblers Fight Back: A Profess','Gamblers Fight Back: A Professional Gambler\\'s Journey of How to Survive and Thrive in the Casino','Greg Elder','Y',4.29,21,95,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (16,'The Frugal Gambler','The Frugal Gambler','Jean Scott','N',3.74,35,94,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (17,'Sharp Sports Betting','Sharp Sports Betting','Stanford Wong','N',3.66,134,93,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (18,'The Man with the $100,000 Brea','The Man with the $100,000 Breasts: And Other Gambling Stories','Michael Konik','Y',3.75,189,92,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (19,'Lay the Favorite: A Memoir of ','Lay the Favorite: A Memoir of Gambling','Beth Raymer','N',3.42,707,91,1,0,0)";
  mysql_conn.query(sql, result);
  sql = "INSERT INTO gambling_books VALUES (20,'Gamblers Fight Back: A Profess','Gamblers Fight Back: A Professional Gambler\\'s Journey of How to Survive and Thrive in the Casino','Greg Elder','Y',4.29,21,90,1,0,0)";
  mysql_conn.query(sql, result);

  LOG(logINFO) << "init DB '" + database_ + "' done";
}

ostream &operator<<(ostream &os, const request &req)
{
  os << req.method << '|' << req.uri << '|' << req.http_version;
  for(auto v : req.headers)
    os << '|' << v.name << '=' << v.value;
  return os;
}

ostream &operator<<(ostream &os, const reply &rep)
{
  os << rep.status_code;
  for(auto v : rep.headers)
    os << '|' << v.name << '=' << v.value;
  os << '|' << rep.content.substr(0, 255) << "..";
  return os;
}

void request_handler::handle_request(const request &req, reply &rep)
{
  LOG(logINFO) << "handle_request req " << req;

  try
  {
    if (req.method == "GET")
      handle_get(req, rep);
    else if (req.method == "HEAD")
      handle_head(req, rep);
    else if (req.method == "POST")
      handle_post(req, rep);
    else if (req.method == "PUT")
      handle_put(req, rep);
    else if (req.method == "PATCH")
      handle_patch(req, rep);
    else if (req.method == "DELETE")
      handle_delete(req, rep);
    else if (req.method == "OPTIONS")
      handle_options(req, rep);
  }
  catch ( const std::exception &e)
  {
    LOG(logERROR) << "handle_request() exception: " << e.what();
    rep = reply::make_reply(http_status::expectation_failed, e.what());
  }

  LOG(logINFO) << "handle_request rep " << rep;
}

void request_handler::handle_get(const request &req, reply &rep)
{
  if (req.uri == "/") {
      rep = reply::make_reply(http_status::ok, "Hello!\ntry type: http://127.0.0.1:3300/test");
  }
  else if (req.uri == "/test") {
      rep = reply::make_reply(http_status::ok, "TEST OK");
  }
  else if (req.uri == "/books") {
      rep = get_select_view(
        "SELECT gambling_book_id id, short_name book, author, rating_avg, ratings, score, person_voted "
        "FROM gambling_books");
  }
  else
    rep = reply::reply_status_only(http_status::not_found);
}

void request_handler::handle_head(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

void request_handler::handle_post(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

void request_handler::handle_put(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

void request_handler::handle_patch(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

void request_handler::handle_delete(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

void request_handler::handle_options(const request &, reply &rep)
{
  rep = reply::reply_status_only(http_status::not_implemented);
}

reply request_handler::get_select_view(const string &sql)
{
  try
  {
    ip::tcp::endpoint ep(
          ip::address::from_string(db_host),
          db_host_port
          );

    mysql::handshake_params params(
          db_user_name,
          db_password,
          database_,
          mysql::collation::utf8_general_ci,
          mysql::ssl_mode::disable);

    mysql::tcp_connection mysql_conn(context_);
    mysql_conn.connect(ep, params);

    mysql::resultset result;

    mysql_conn.query(sql, result);

    ostringstream str;

    int width = 18;

    str << setw((width + 2) * result.meta().size()) << setfill('-') << '-';
    str << setfill(' ') << '\n';

    for (boost::mysql::metadata meta : result.meta())
    {
        str << setw(1) << ' ';
        str << setw(width) << left << meta.column_name().substr(0, width) << ":";
    }

    str << endl << setw((width + 2) * result.meta().size()) << setfill('-') << '-';
    str << setfill(' ') << '\n';

    for (boost::mysql::row_view row : result.rows())
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
          str << setw(width);
          if (row.at(i).is_string())
            str << left << row.at(i).as_string().substr(0, width);
          else   
            str << right << row.at(i);
          str << " :";
        }
        str << endl;
    }

    str << setw((width + 2) * result.meta().size()) << setfill('-') << '-';
    str << setfill(' ') << endl;

    return reply::make_reply(http_status::ok, str.str());
  } // try
  catch (const boost::mysql::server_error& err)
  {
    LOG(logERROR) << "get_select_view() Error: " << err.what() << ", error code: " << err.code()
                  << " Server diagnostics: " << err.diagnostics().message();
    return reply::make_reply(http_status::expectation_failed, err.what());
  }
}
