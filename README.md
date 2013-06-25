# SGAL project

The Scene Graph Algorithms Library (SGAL) is a toolkit that supports the
construction and maintenance of directed acyclic graphs that represent
scenes and models in three-dimensional space. The toolkit consists of
two libraries, namely SGAL and SCGAL, and an application, called player,
which enables users to visualize the scene in an interactive manner. The
player parses input files that describe the scene in a degenerate yet
extended VRML format. The format is degenerate, as not all VRML features
are supported (yet). However, it has been extended to support several
new features. For example, it has been extended with several geometry
node types, such as the Assembly node type, which represents assemblies
or subassemblies; the AssemblyPart node type, which represents parts of
assemblies; the SphericalGaussianMap node type, which represents
(spherical) Gaussian maps of convex polytopes in 3D, and the
ArrangementOnSphere, which represents arrangements of geodesic arcs
embedded on the sphere. The extended format also supports, for example, a
node type called ExactCoordinate, which represents exact coordinates, and
enables the provision of exact rational coordinates as input. Each node
object of the three types AssemblyPart, SphericalGaussianMap, and
ArrangementOnSphere internally maintains the CGAL data structure that
represents an arrangement of geodesic arcs embedded on the sphere.
