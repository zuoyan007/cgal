#define CGAL_CHECK_EXACTNESS
#define CGAL_CHECK_EXPENSIVE

#include <CGAL/Kinetic/basic.h>

#include <CGAL/Kinetic/Exact_simulation_traits_2.h>
#include <CGAL/Kinetic/Delaunay_triangulation_2.h>
#include <CGAL/Kinetic/Delaunay_triangulation_event_log_visitor_2.h>
#include <algorithm>
#include <iterator>

int main(int, char *[])
{

  typedef CGAL::Kinetic::Exact_simulation_traits_2 Simulation_traits;
  typedef Simulation_traits::Kinetic_kernel::Point_2 Moving_point_2;
  typedef CGAL::Kinetic::Delaunay_triangulation_event_log_visitor_2 Visitor;
  typedef CGAL::Kinetic::Delaunay_triangulation_2<Simulation_traits, Visitor> KDel;

  Simulation_traits simtr;
  Simulation_traits::Simulator::Handle sp= simtr.simulator_handle();

  KDel kdel(simtr);
  kdel.triangulation(3).finite_vertices_begin();
  
  //CGAL_KINETIC_SET_LOG_LEVEL(CGAL::Kinetic::LOG_LOTS);
 
  std::ifstream in("data/Delaunay_triangulation_2.input");
  if (!in) {
    std::cerr << "Error opening input file: " << "data/Delaunay_triangulation_2.input" << std::endl;
    return EXIT_FAILURE;
  }
  char buf[1000];
  int nread=0;
  while (true ) {
    in.getline(buf, 1000);
    if (!in) break;
    std::istringstream il(buf);
    Simulation_traits::Kinetic_kernel::Point_2 p;
    il >> p;
    //std::cout << p << std::endl;
    simtr.active_points_2_table_handle()->insert(p);
    ++nread;
  }

  kdel.audit();

  while (simtr.simulator_handle()->next_event_time()
	 < simtr.simulator_handle()->end_time()) {
    sp->set_current_event_number(sp->current_event_number()+1);
  }

  /*std::copy(kdel.visitor().begin(), kdel.visitor().end(),
    std::ostream_iterator<std::string>(std::cout, "\n"));*/

  std::ifstream out("data/Delaunay_triangulation_2.output");
  if (!out) {
    std::cerr << "Error opening input file: " << "data/Delaunay_triangulation_2.output" << std::endl;
    return EXIT_FAILURE;
  }

  int error_count=0;
  for (CGAL::Kinetic::Delaunay_triangulation_event_log_visitor_2::Event_iterator it = kdel.visitor().events_begin();
       it != kdel.visitor().events_end(); ++it) {
    char buf[1000];
    out.getline(buf, 1000);
    if (*it != buf) {
      std::cerr << "ERROR Got event: " << *it << std::endl;
      std::cerr << "ERROR Expected event: " << buf << std::endl;
      ++error_count;
    }
  }

  while (true) {
    if (!out) break;
    char buf[1000];
    out.getline(buf, 1000);
    if (out) {
      std::cerr << "ERROR Missing event: " << buf << std::endl;
      ++error_count;
    }
  }

  if (error_count != 0) {
    std::cerr << "ERROR " << error_count << " errors in " << kdel.visitor().size() << " events.\n";
  } else {
    std::cout << "No errors for " << std::distance(kdel.visitor().events_begin(), kdel.visitor().events_end()) << " events.\n";
  }
  
  if (CGAL::Kinetic::internal::fail__) return EXIT_FAILURE;
  else return EXIT_SUCCESS;
};
