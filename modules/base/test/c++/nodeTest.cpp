#include <cxxtest/TestDrive.h>

#include "nodeTest_def.h"

CXXTEST_SUITE(NodeTest)
{
  CXXTEST_TEST(modified)
  {
    ma::Node root("root");
    root.modified(); // To grab the last modified timestamp;
    unsigned long ts = root.timestamp();
    
    ma::Node branchA("branchA",&root);
    ma::Node leafA1("leafA1",&branchA);
    ma::Node leafA2("leafA2",&branchA);
    ma::Node branchB("branchB",&root);
    ma::Node leafB1("leafB1",&branchB);
    ma::Node leafB2("leafB2",&branchB);
    
    TS_ASSERT_EQUALS(root.timestamp(),ts+10ul);
    TS_ASSERT_EQUALS(branchA.timestamp(),ts+4ul);
    TS_ASSERT_EQUALS(leafA1.timestamp(),0ul);
    TS_ASSERT_EQUALS(leafA2.timestamp(),0ul);
    TS_ASSERT_EQUALS(branchB.timestamp(),ts+9ul);
    TS_ASSERT_EQUALS(leafB1.timestamp(),0ul);
    TS_ASSERT_EQUALS(leafB2.timestamp(),0ul);
  };
  
  CXXTEST_TEST(staticProperty)
  {
    ma::Node node("foo");
    
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    unsigned long ts = node.timestamp();
    node.setName("bar");
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.name(),"bar");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"bar");
    ts = node.timestamp();
    node.setProperty("name",std::string("foo"));
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    ts = node.timestamp();
    node.setProperty("name","foo");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    node.setName("foo");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.description(),"");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"");
    node.setDescription("I am an empty description, isn't it?");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.description(),"I am an empty description, isn't it?");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"I am an empty description, isn't it?");
    node.setProperty("description",std::string("Or was I..."));
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.description(),"Or was I...");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"Or was I...");
  };
  
  CXXTEST_TEST(dynamicProperty)
  {
    ma::Node node("foo");
    unsigned long ts = node.timestamp();
    node.setProperty("bar",45);
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.property("bar"),45);
    node.setProperty("bar",45);
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    ts = node.timestamp();
    node.setProperty("bar",std::string("hello"));
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    ts = node.timestamp();
    node.setProperty("bar","hello");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.property("bar").cast<std::string>(),"hello");
    node.setProperty("bar",ma::Any());
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.property("bar").isValid(),false);
    TS_ASSERT_EQUALS(node.property("my_unknown_dynamic_property").isValid(),false);
  };
  
  CXXTEST_TEST(inheritingClassWithStaticProperty)
  {
    TestNode node("foo");
    
    TS_ASSERT_EQUALS(node.name(),"foo");
    auto n = node.property("name");
    TS_ASSERT_EQUALS(n.isValid(), true);
    TS_ASSERT_EQUALS(n.isEmpty(), false);
    TS_ASSERT(n == "foo");
    TS_ASSERT_EQUALS(n.cast<std::string>(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    node.setName("bar");
    TS_ASSERT_EQUALS(node.name(),"bar");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"bar");
    node.setProperty("name",std::string("foo"));
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    
    TS_ASSERT_EQUALS(node.version(),1);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),1);
    node.setVersion(2);
    TS_ASSERT_EQUALS(node.version(),2);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),2);
    node.setProperty("version",3);
    TS_ASSERT_EQUALS(node.version(),3);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),3);
    
    ma::Node* bar = new TestNode("bar",&node);
    TS_ASSERT_EQUALS(static_cast<TestNode*>(bar)->version(),1);
    TS_ASSERT_EQUALS(bar->property("version").cast<int>(),1);
  };
  
  CXXTEST_TEST(childrenStack)
  {
    TestNode root("root");
    TS_ASSERT_EQUALS(root.count(),1);
    TestNode leafA("leafA",&root);
    TS_ASSERT_EQUALS(root.count(),2);
    TestNode leafB("leafB",&root);
    TS_ASSERT_EQUALS(root.count(),3);
    TestNode leafC("leafC",&root);
    TS_ASSERT_EQUALS(root.count(),4);
    ma::Node leafD("leafD",&root);
    TS_ASSERT_EQUALS(root.count(),4);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.children().front(),&leafA);
    TS_ASSERT_EQUALS(root.children().back(),&leafD);
    TS_ASSERT_EQUALS(leafA.hasChildren(),false);
    TS_ASSERT_EQUALS(leafA.hasParents(),true);
    TS_ASSERT_EQUALS(leafA.parents().front(),&root);
    TS_ASSERT_EQUALS(leafB.hasChildren(),false);
    TS_ASSERT_EQUALS(leafB.hasParents(),true);
    TS_ASSERT_EQUALS(leafB.parents().front(),&root);
    TS_ASSERT_EQUALS(leafC.hasChildren(),false);
    TS_ASSERT_EQUALS(leafC.hasParents(),true);
    TS_ASSERT_EQUALS(leafC.parents().front(),&root);
  };
  
  CXXTEST_TEST(childrenHeap)
  {
    TestNode root("root");
    ma::Node* foo = new ma::Node("foo",&root);
    TS_ASSERT_DIFFERS(root.timestamp(),0ul);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.children().front(),foo);
    delete foo;
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    foo = new TestNode("foo2",&root);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    new TestNode("bar",foo);
    TS_ASSERT_EQUALS(foo->hasChildren(),true);
    TS_ASSERT_EQUALS(foo->children().front()->name(),"bar");
    TestNode* col = new TestNode("col",foo);
    ma::Node* sun = new ma::Node("sun",foo);
    new TestNode("moon",foo);
    new TestNode("moon",foo);
    new TestNode("moon",foo);
    new TestNode("planet1",sun);
    new TestNode("planet2",sun);
    new TestNode("planet3",sun);
    
    TS_ASSERT_EQUALS(root.findChild("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild(),foo);
    TS_ASSERT_EQUALS(root.findChild("foo"),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild<const TestNode*>("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("foo"),nullptr);

    TS_ASSERT_EQUALS(foo->findChild("col"),col);
    TS_ASSERT_EQUALS(foo->findChild("allo"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild("col"),col);
    TS_ASSERT_EQUALS(foo->findChild("allo"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild<TestNode*>("col"),col);
    TS_ASSERT_DIFFERS(foo->findChild<TestNode*>(),col); // Should be "bar"

    TS_ASSERT_EQUALS(foo->findChild("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<ma::Node*>("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<const ma::Node*>("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<TestNode*>("sun"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild<const TestNode*>("sun"),nullptr);

    TS_ASSERT_EQUALS(root.findChild("sun"),sun);
    TS_ASSERT_EQUALS(root.findChild("sun",{},false),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("sun"),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("col"),col);
    TS_ASSERT_EQUALS(root.findChild("col"),col);
    TS_ASSERT_EQUALS(root.findChild("col",{},false),nullptr);
    
    std::vector<ma::Node*> moonsA = root.findChildren("moon");
    TS_ASSERT_EQUALS(moonsA.size(), 3u);
    
    std::vector<TestNode*> moonsB = root.findChildren<TestNode*>("moon");
    TS_ASSERT_EQUALS(moonsB.size(), 3u);
    
    std::vector<ma::Node*> planetsA = root.findChildren(std::regex("planet.*"));
    TS_ASSERT_EQUALS(planetsA.size(), 3u);
    
    std::vector<TestNode*> planetsB = sun->findChildren<TestNode*>(std::regex("planet.*"),{},false);
    TS_ASSERT_EQUALS(planetsB.size(), 3u);
    
    std::vector<ma::Node*> all = root.findChildren();
    TS_ASSERT_EQUALS(all.size(), 10u);
  };
  
  CXXTEST_TEST(childMethod)
  {
    TestNode root("root");
    TS_ASSERT_EQUALS(root.count(),1);
    TestNode leafA("leafA",&root);
    TS_ASSERT_EQUALS(root.count(),2);
    TestNode leafB("leafB",&root);
    TS_ASSERT_EQUALS(root.count(),3);
    TestNode leafC("leafC",&root);
    TS_ASSERT_EQUALS(root.count(),4);
    ma::Node leafD("leafD",&root);
    TS_ASSERT_EQUALS(root.child(0),&leafA);
    TS_ASSERT_EQUALS(root.child<TestNode*>(0),&leafA);
    TS_ASSERT_EQUALS(root.child(1),&leafB);
    TS_ASSERT_EQUALS(root.child<TestNode*>(1),&leafB);
    TS_ASSERT_EQUALS(root.child(2),&leafC);
    TS_ASSERT_EQUALS(root.child<TestNode*>(2),&leafC);
    TS_ASSERT_EQUALS(root.child(3),&leafD);
    TS_ASSERT_EQUALS(root.child<ma::Node*>(3),&leafD);
    TS_ASSERT_EQUALS(root.child<TestNode*>(3),nullptr);
    TS_ASSERT_EQUALS(root.child(4),nullptr);
  };
  
  CXXTEST_TEST(addParent)
  {
    TestNode root("root");
    TestNode leafA("leafA");
    TS_ASSERT_EQUALS(leafA.hasChildren(),false);
    TS_ASSERT_EQUALS(leafA.hasParents(),false);
    leafA.addParent(nullptr);
    TS_ASSERT_EQUALS(leafA.hasParents(),false);
    TS_ASSERT_EQUALS(leafA.hasChildren(),false);
    leafA.addParent(&root);
    TS_ASSERT_EQUALS(leafA.hasParents(),true);
    TS_ASSERT_EQUALS(leafA.hasChildren(),false);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
  };
  
  CXXTEST_TEST(removeParent)
  {
    TestNode root("root");
    TestNode leafA("leafA",&root);
    TS_ASSERT_EQUALS(leafA.hasParents(),true);
    leafA.removeParent(nullptr);
    TS_ASSERT_EQUALS(leafA.hasParents(),true);
    leafA.removeParent(&root);
    TS_ASSERT_EQUALS(leafA.hasParents(),false);
    TS_ASSERT_EQUALS(root.hasChildren(),false);
  };
  
  CXXTEST_TEST(clone)
  {
    TestNode root("root");
    root.setProperty("bar",45);
    root.setDescription("bla bla");
    ma::Node* root_ = root.clone();
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root_->hasChildren(),false);
    TS_ASSERT_EQUALS(root_->hasParents(),false);
    TS_ASSERT_EQUALS(root_->property("bar").cast<int>(),45);
    TS_ASSERT_EQUALS(root_->description(),"bla bla");
    TestNode* test = ma::node_cast<TestNode*>(root_);
    TS_ASSERT_DIFFERS(test,nullptr);
    TS_ASSERT_EQUALS(test->name(), "root");
    TS_ASSERT_EQUALS(test->version(), 1);
    test->setName("foo");
    test->setVersion(2);
    TS_ASSERT_EQUALS(root.name(), "root");
    TS_ASSERT_EQUALS(root.version(), 1);
    TS_ASSERT_EQUALS(test->name(), "foo");
    TS_ASSERT_EQUALS(test->version(), 2);
    delete root_;
  };
  
  CXXTEST_TEST(cloneWithChildren)
  {
    TestNode root("root");
    TestNode branch("branch",&root);
    ma::Node leaf("leaf",&branch);
    
    ma::Node* root_ = root.clone();
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root_->hasChildren(),true);
    TS_ASSERT_EQUALS(root_->hasParents(),false);
    TestNode* testRoot = ma::node_cast<TestNode*>(root_);
    TestNode* testBranch = testRoot->findChild<TestNode*>("branch");
    ma::Node* testLeaf = testBranch->findChild<ma::Node*>("leaf");
    TS_ASSERT_DIFFERS(testRoot,nullptr);
    TS_ASSERT_EQUALS(testRoot->name(), "root");
    TS_ASSERT_EQUALS(testRoot->version(), 1);
    TS_ASSERT_DIFFERS(testBranch,nullptr);
    TS_ASSERT_EQUALS(testBranch->name(), "branch");
    TS_ASSERT_EQUALS(testBranch->version(), 1);
    TS_ASSERT_DIFFERS(testLeaf,nullptr);
    TS_ASSERT_EQUALS(testLeaf->name(), "leaf");
    delete root_;
  };
  
  CXXTEST_TEST(cloneWithRoot)
  {
    TestNode root("root");
    ma::Node* child = root.clone(&root); // Strange but that's work!
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(child->hasChildren(),false);
    TS_ASSERT_EQUALS(child->hasParents(),true);
    TestNode* test = ma::node_cast<TestNode*>(child);
    TS_ASSERT_DIFFERS(test,nullptr);
    TS_ASSERT_EQUALS(test->name(), "root");
    TS_ASSERT_EQUALS(test->version(), 1);
    test->setName("child");
    test->setVersion(2);
    TS_ASSERT_EQUALS(root.name(), "root");
    TS_ASSERT_EQUALS(root.version(), 1);
    TS_ASSERT_EQUALS(test->name(), "child");
    TS_ASSERT_EQUALS(test->version(), 2);
  };
  
  CXXTEST_TEST(copy)
  {
    TestNode root("root");
    root.setVersion(10);
    TestNode branch("branch",&root);
    branch.setProperty("version",20);
    ma::Node leaf("leaf",&branch);
    leaf.setProperty("version",30);
    TestNode root_("copy");
    root_.copy(&root);
    
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root_.hasChildren(),true);
    TS_ASSERT_EQUALS(root_.hasParents(),false);
    TestNode* testRoot = ma::node_cast<TestNode*>(&root_);
    TestNode* testBranch = testRoot->findChild<TestNode*>("branch");
    ma::Node* testLeaf = testBranch->findChild<ma::Node*>("leaf");
    TS_ASSERT_DIFFERS(testRoot,nullptr);
    TS_ASSERT_EQUALS(testRoot->name(), "root");
    TS_ASSERT_EQUALS(testRoot->version(), 10);
    TS_ASSERT_DIFFERS(testBranch,nullptr);
    TS_ASSERT_EQUALS(testBranch->name(), "branch");
    TS_ASSERT_EQUALS(testBranch->version(), 20);
    TS_ASSERT_DIFFERS(testLeaf,nullptr);
    TS_ASSERT_EQUALS(testLeaf->name(), "leaf");
    TS_ASSERT_EQUALS(testLeaf->property("version").cast<int>(), 30);
  };
  
  CXXTEST_TEST(copyAndParent)
  {
    ma::Node root("root");
    TestNode leaf("leaf",&root);
    TestNode foo("foo");
    foo.setVersion(99);
    leaf.copy(&foo);
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(leaf.hasChildren(),false);
    TS_ASSERT_EQUALS(leaf.hasParents(),false);
    TS_ASSERT_EQUALS(leaf.name(),"foo");
    TS_ASSERT_EQUALS(leaf.version(),99);
  }
  
  CXXTEST_TEST(retrievePath)
  {
    ma::Node root("root");
    TestNode branch1("branch1",&root);
    ma::Node leaf1("leaf1",&branch1);
    TestNode branch2("branch2",&root);
    ma::Node branch2_1("branch2_1",&branch2);
    TestNode leaf2_1_1("leaf2_1_1",&branch2_1);
    
    std::vector<const ma::Node*> ref;
    
    auto path1 = root.retrievePath(&branch1);
    TS_ASSERT_EQUALS(path1.size(), 2u);
    ref = {&root,&branch1};
    TS_ASSERT(path1 == ref);
    
    auto path2 = root.retrievePath(&leaf1);
    TS_ASSERT_EQUALS(path2.size(), 3u);
    ref = {&root,&branch1,&leaf1};
    TS_ASSERT(path2 == ref);
    
    auto path3 = branch1.retrievePath(&leaf1);
    TS_ASSERT_EQUALS(path3.size(), 2u);
    ref = {&branch1,&leaf1};
    TS_ASSERT(path3 == ref);
    
    auto path4 = root.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path4.size(), 4u);
    ref = {&root,&branch2,&branch2_1,&leaf2_1_1};
    TS_ASSERT(path4 == ref);
    
    auto path5 = branch2.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path5.size(), 3u);
    ref = {&branch2,&branch2_1,&leaf2_1_1};
    TS_ASSERT(path5 == ref);
    
    auto path6 = leaf1.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path6.size(), 0u);
  };
  
  CXXTEST_TEST(isCastable)
  {
    ma::Node temp1("temp1");
    TestNode temp2("temp1");
    ma::Node* temp3 = new TestNode("temp3");
    TS_ASSERT_EQUALS(temp1.isCastable(ma::static_typeid<ma::Node>()), true);
    TS_ASSERT_EQUALS(temp1.isCastable(ma::static_typeid<TestNode>()), false);
    TS_ASSERT_EQUALS(temp2.isCastable(ma::static_typeid<ma::Node>()), true);
    TS_ASSERT_EQUALS(temp2.isCastable(ma::static_typeid<TestNode>()), true);
    TS_ASSERT_EQUALS(temp3->isCastable(ma::static_typeid<ma::Node>()), true);
    TS_ASSERT_EQUALS(temp3->isCastable(ma::static_typeid<TestNode>()), true);
    delete temp3;
  };
  
  CXXTEST_TEST(externInheriting)
  {
    ma::Node temp1("temp1");
    TestNode2 temp2("temp2",&temp1);
    TS_ASSERT_EQUALS(temp1.isCastable(ma::static_typeid<ma::Node>()), true);
    TS_ASSERT_EQUALS(temp1.isCastable(ma::static_typeid<TestNode2>()), false);
    TS_ASSERT_EQUALS(temp2.isCastable(ma::static_typeid<ma::Node>()), true);
    TS_ASSERT_EQUALS(temp2.isCastable(ma::static_typeid<TestNode2>()), true);
    
    temp2.setProperty("name","foo");
    TS_ASSERT_EQUALS(temp2.name(),"foo");
    TS_ASSERT_EQUALS(temp2.property("name").cast<std::string>(),"foo");
    temp2.setDescription("something");
    TS_ASSERT_EQUALS(temp2.description(),"something");
    TS_ASSERT_EQUALS(temp2.property("description").cast<std::string>(),"something");
    
    TestNode2 foo("bar");
    foo.copy(&temp2);
    foo.setVersion(99);
    TS_ASSERT_EQUALS(temp2.hasChildren(),false);
    TS_ASSERT_EQUALS(temp2.hasParents(),true);
    TS_ASSERT_EQUALS(foo.hasChildren(),false);
    TS_ASSERT_EQUALS(foo.hasParents(),false);
    TS_ASSERT_EQUALS(foo.name(),"foo");
    TS_ASSERT_EQUALS(temp2.version(),2);
    TS_ASSERT_EQUALS(foo.version(),99);
  };
};

CXXTEST_SUITE_REGISTRATION(NodeTest)
CXXTEST_TEST_REGISTRATION(NodeTest, modified)
CXXTEST_TEST_REGISTRATION(NodeTest, staticProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, dynamicProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, inheritingClassWithStaticProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, childrenStack)
CXXTEST_TEST_REGISTRATION(NodeTest, childrenHeap)
CXXTEST_TEST_REGISTRATION(NodeTest, childMethod)
CXXTEST_TEST_REGISTRATION(NodeTest, addParent)
CXXTEST_TEST_REGISTRATION(NodeTest, removeParent)
CXXTEST_TEST_REGISTRATION(NodeTest, clone)
CXXTEST_TEST_REGISTRATION(NodeTest, cloneWithChildren)
CXXTEST_TEST_REGISTRATION(NodeTest, cloneWithRoot)
CXXTEST_TEST_REGISTRATION(NodeTest, copy)
CXXTEST_TEST_REGISTRATION(NodeTest, copyAndParent)
CXXTEST_TEST_REGISTRATION(NodeTest, retrievePath)
CXXTEST_TEST_REGISTRATION(NodeTest, isCastable)
CXXTEST_TEST_REGISTRATION(NodeTest, externInheriting)