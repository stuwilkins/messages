#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <aSubRecord.h>

struct upload {
  char **data;
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload *upload_ctx = (struct upload *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = upload_ctx->data[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;
    ((char *)ptr)[len] = '\r';
    ((char *)ptr)[len+1] = '\n';
    return len + 2;
  }

  return 0;
}

int mail(char *from, char *to, char *message[]) {

  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload upload_ctx;
  int rtn = 0;
  char *srv = NULL;

  if((srv = getenv("MAIL_SERVER")) == NULL){
    return 1;
  }

  curl = curl_easy_init();
  if(!curl){
    return 1;
  }

  upload_ctx.data = message;
  upload_ctx.lines_read = 0;

  /* This is the URL for your mailserver */
  curl_easy_setopt(curl, CURLOPT_URL, srv);

  curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
  recipients = curl_slist_append(recipients, to);
  curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
  curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

  /* Send the message */
  res = curl_easy_perform(curl);

  /* Check for errors */
  if(res != CURLE_OK){
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    rtn = 2;
  }

  /* Free the list of recipients */
  curl_slist_free_all(recipients);
  curl_easy_cleanup(curl);

  return rtn;
}

static long messages(aSubRecord *prec) {

  // Create the message from the input fields
  
  long mode = *((long *)prec->u);
  long rtn_mode = 0;
  
  char *message[26];

  char *to      = getenv("MAIL_TO");
  char *from    = getenv("MAIL_FROM");
  char *subject = getenv("MAIL_SUBJECT");
  char *srv     = getenv("MAIL_SERVER");

  if(!to || !from || !subject || !srv){
    return 3;
  }
 
  if(mode & 0x1){
    // Send E-Mail
    char dateline[128];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dateline, sizeof(dateline)-1, "Date: %d %m %Y %H:%M", t);
    char toline[128];
    snprintf(toline, sizeof(toline)-1, "To: %s", to);
    char fromline[128];
    snprintf(fromline, sizeof(fromline)-1, "From: %s", from);
    char subjectline[128];
    snprintf(subjectline, sizeof(subjectline)-1, "Subject: %s", subject);

    message[0] = dateline;
    message[1] = toline;
    message[2] = fromline;
    message[3] = subjectline;
    message[4] = "\r\n";

    int cnt = 5;
    if(prec->noa > 1) message[cnt++] = (char *)prec->a;
    if(prec->nob > 1) message[cnt++] = (char *)prec->b;
    if(prec->noc > 1) message[cnt++] = (char *)prec->c;
    if(prec->nod > 1) message[cnt++] = (char *)prec->d;
    if(prec->noe > 1) message[cnt++] = (char *)prec->e;
    if(prec->nof > 1) message[cnt++] = (char *)prec->f;
    if(prec->nog > 1) message[cnt++] = (char *)prec->g;
    if(prec->noh > 1) message[cnt++] = (char *)prec->h;
    if(prec->noi > 1) message[cnt++] = (char *)prec->i;
    if(prec->noj > 1) message[cnt++] = (char *)prec->j;
    if(prec->nok > 1) message[cnt++] = (char *)prec->k;
    if(prec->nol > 1) message[cnt++] = (char *)prec->l;
    if(prec->nom > 1) message[cnt++] = (char *)prec->m;
    if(prec->non > 1) message[cnt++] = (char *)prec->n;
    if(prec->noo > 1) message[cnt++] = (char *)prec->o;
    if(prec->nop > 1) message[cnt++] = (char *)prec->p;
    if(prec->noq > 1) message[cnt++] = (char *)prec->q;
    if(prec->nor > 1) message[cnt++] = (char *)prec->r;
    if(prec->nos > 1) message[cnt++] = (char *)prec->s;
    
    message[cnt] = NULL;

    int rtn = mail(from, to, message);

    if(!rtn){
      rtn_mode |= 0x1;
    }

  }

  // If sucsessful process output 
  // if selected by the operator

  if(mode & 0x2){
  
    strncpy((char*)prec->vala, (char*)prec->a, prec->nova);
    strncpy((char*)prec->valb, (char*)prec->b, prec->novb);
    strncpy((char*)prec->valc, (char*)prec->c, prec->novc);
    strncpy((char*)prec->vald, (char*)prec->d, prec->novd);
    strncpy((char*)prec->vale, (char*)prec->e, prec->nove);
    strncpy((char*)prec->valf, (char*)prec->f, prec->novf);
    strncpy((char*)prec->valg, (char*)prec->g, prec->novg);
    strncpy((char*)prec->valh, (char*)prec->h, prec->novh);
    strncpy((char*)prec->vali, (char*)prec->i, prec->novi);
    strncpy((char*)prec->valj, (char*)prec->j, prec->novj);
    strncpy((char*)prec->valk, (char*)prec->k, prec->novk);
    strncpy((char*)prec->vall, (char*)prec->l, prec->novl);
    strncpy((char*)prec->valm, (char*)prec->m, prec->novm);
    strncpy((char*)prec->valn, (char*)prec->n, prec->novn);
    strncpy((char*)prec->valo, (char*)prec->o, prec->novo);
    strncpy((char*)prec->valp, (char*)prec->p, prec->novp);
    strncpy((char*)prec->valq, (char*)prec->q, prec->novq);
    strncpy((char*)prec->valr, (char*)prec->r, prec->novr);
    strncpy((char*)prec->vals, (char*)prec->s, prec->novs);

    rtn_mode |= 0x2;

  }

  return rtn_mode;
}

epicsRegisterFunction(messages);
