#include <sys/param.h>
#include <sys/systm.h>

#include <sys/map.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/mbuf.h>
#include <sys/acct.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <ufs/ufs/quota.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <sys/protosw.h>
#include <sys/domain.h>
#include <sys/signalvar.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_var.h>

void ipintr();

struct socket* listenon(unsigned short port)
{
  struct socket* so = NULL;
  socreate(AF_INET, &so, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  bzero(&addr, sizeof addr);
  addr.sin_len = sizeof addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  struct mbuf* nam = m_devget((caddr_t)&addr, sizeof addr, 0, NULL, NULL);
  sobind(so, nam);
  solisten(so, 5);
  return so;
}

void handshake()
{
  int port = 1234;
  listenon(port);
  struct socket* so = NULL;
  socreate(AF_INET, &so, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  bzero(&addr, sizeof addr);
  addr.sin_len = sizeof addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(0x7f000001);
  struct mbuf* nam = m_devget((caddr_t)&addr, sizeof addr, 0, NULL, NULL);
  soconnect(so, nam);
  ipintr();
}

