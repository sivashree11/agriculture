/**
 * ============================================================================
 * Project Title   : Smart Agricultural By-Product Marketplace
 * Academic Level  : Second-Year Computer Science & Engineering Mini Project
 * Technologies    : C++ (Standard Libraries, Sockets, fstream), HTML5, CSS3
 * Strict Constraint: ZERO JavaScript (100% Native HTML Forms & C++ Server)
 * Compatibility   : Linux / Ubuntu (g++) and Windows (MinGW / MSYS2 g++)
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstring>

// Cross-Platform Socket Headers (Ubuntu / Linux & Windows)
#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  typedef int socklen_t;
  #define CLOSE_SOCKET closesocket
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define SOCKET int
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  #define CLOSE_SOCKET close
#endif

// ============================================================================
// 1. OBJECT-ORIENTED PROGRAMMING: CLASS HIERARCHY & DATA ENCAPSULATION
// ============================================================================

/**
 * Base Class: User
 * Demonstrates: Encapsulation, Abstraction, and Polymorphism (Virtual Function)
 */
class User {
protected:
    std::string name;
    std::string contactNumber;
    std::string location;

public:
    User() : name(""), contactNumber(""), location("") {}
    User(const std::string& n, const std::string& contact, const std::string& loc)
        : name(n), contactNumber(contact), location(loc) {}

    virtual ~User() {}

    // Getters (Encapsulation)
    std::string getName() const { return name; }
    std::string getContactNumber() const { return contactNumber; }
    std::string getLocation() const { return location; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setContactNumber(const std::string& c) { contactNumber = c; }
    void setLocation(const std::string& loc) { location = loc; }

    // Polymorphic Virtual Function: Overridden in derived classes
    virtual std::string getRole() const = 0;
};

/**
 * Derived Class: Farmer (Inherits from User)
 * Demonstrates: Single Inheritance and Polymorphism
 */
class Farmer : public User {
private:
    std::string farmDistrict;

public:
    Farmer() : User(), farmDistrict("") {}
    Farmer(const std::string& n, const std::string& contact, const std::string& loc, const std::string& dist)
        : User(n, contact, loc), farmDistrict(dist) {}

    std::string getFarmDistrict() const { return farmDistrict; }

    // Function Overriding (Polymorphism)
    std::string getRole() const override {
        return "Farmer / Agricultural Producer";
    }
};

/**
 * Derived Class: IndustrialBuyer (Inherits from User)
 * Demonstrates: Inheritance, Overriding, and Custom Business Attributes
 */
class IndustrialBuyer : public User {
private:
    std::string companyName;
    std::string industryType;
    std::string targetMaterial;
    double requiredQuantityKg;
    double maxBudgetPerKg;

public:
    IndustrialBuyer()
        : User(), companyName(""), industryType(""), targetMaterial(""),
          requiredQuantityKg(0.0), maxBudgetPerKg(0.0) {}

    IndustrialBuyer(const std::string& n, const std::string& contact, const std::string& loc,
                    const std::string& comp, const std::string& ind, const std::string& target,
                    double reqQty, double maxBudget)
        : User(n, contact, loc), companyName(comp), industryType(ind),
          targetMaterial(target), requiredQuantityKg(reqQty), maxBudgetPerKg(maxBudget) {}

    // Getters
    std::string getCompanyName() const { return companyName; }
    std::string getIndustryType() const { return industryType; }
    std::string getTargetMaterial() const { return targetMaterial; }
    double getRequiredQuantityKg() const { return requiredQuantityKg; }
    double getMaxBudgetPerKg() const { return maxBudgetPerKg; }

    // Function Overriding (Polymorphism)
    std::string getRole() const override {
        return "Industrial Buyer / Bio-Manufacturer";
    }
};

/**
 * Class: AgriculturalWaste
 * Encapsulates all details of an agricultural by-product listing
 */
class AgriculturalWaste {
private:
    int id;
    std::string materialName;
    std::string category;
    double quantityKg;
    double pricePerKg;
    std::string location;
    std::string condition;
    std::string farmerName;
    std::string farmerContact;

public:
    // Static Data Member to track records created in current session
    static int sessionRecordCount;

    AgriculturalWaste()
        : id(0), materialName(""), category(""), quantityKg(0.0),
          pricePerKg(0.0), location(""), condition(""),
          farmerName(""), farmerContact("") {}

    AgriculturalWaste(int wasteId, const std::string& mat, const std::string& cat,
                      double qty, double price, const std::string& loc,
                      const std::string& cond, const std::string& fName,
                      const std::string& fContact)
        : id(wasteId), materialName(mat), category(cat), quantityKg(qty),
          pricePerKg(price), location(loc), condition(cond),
          farmerName(fName), farmerContact(fContact) {}

    // Getters (Encapsulation)
    int getId() const { return id; }
    std::string getMaterialName() const { return materialName; }
    std::string getCategory() const { return category; }
    double getQuantityKg() const { return quantityKg; }
    double getPricePerKg() const { return pricePerKg; }
    std::string getLocation() const { return location; }
    std::string getCondition() const { return condition; }
    std::string getFarmerName() const { return farmerName; }
    std::string getFarmerContact() const { return farmerContact; }

    // Helper Calculation Function
    double calculateTotalValue() const {
        return quantityKg * pricePerKg;
    }

    // Convert object to pipe-delimited string for file storage
    std::string toFileString() const {
        std::ostringstream oss;
        oss << id << "|" << materialName << "|" << category << "|"
            << quantityKg << "|" << pricePerKg << "|" << location << "|"
            << condition << "|" << farmerName << "|" << farmerContact;
        return oss.str();
    }

    // Populate object from pipe-delimited string
    static AgriculturalWaste fromFileString(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> parts;
        while (std::getline(ss, token, '|')) {
            parts.push_back(token);
        }
        if (parts.size() >= 9) {
            int wid = std::stoi(parts[0]);
            double wqty = std::stod(parts[3]);
            double wprice = std::stod(parts[4]);
            return AgriculturalWaste(wid, parts[1], parts[2], wqty, wprice,
                                     parts[5], parts[6], parts[7], parts[8]);
        }
        return AgriculturalWaste();
    }
};

// Initialize Static Member
int AgriculturalWaste::sessionRecordCount = 0;

/**
 * Structure: MatchResult
 * Stores matching supplier details and detailed explanation of match criteria
 */
struct MatchResult {
    AgriculturalWaste listing;
    int score;                      // Match Percentage (0 - 100)
    std::string badgeClass;         // CSS Class for badge styling
    std::vector<std::string> matchReasons;
};

// ============================================================================
// 2. MARKETPLACE ENGINE & FILE PERSISTENCE (fstream)
// ============================================================================

class WasteMarketplace {
private:
    std::vector<AgriculturalWaste> listings;
    std::string dataFilePath;
    int nextId;

    // Helper: Case-insensitive lowercase conversion
    static std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

public:
    WasteMarketplace(const std::string& filename = "listings.txt")
        : dataFilePath(filename), nextId(101) {
        loadFromFile();
    }

    // Load records using std::ifstream
    void loadFromFile() {
        listings.clear();
        std::ifstream inFile(dataFilePath);
        if (!inFile.is_open()) {
            // If file does not exist, initialize with realistic sample data
            seedSampleData();
            saveToFile();
            return;
        }

        std::string line;
        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            AgriculturalWaste waste = AgriculturalWaste::fromFileString(line);
            if (waste.getId() > 0) {
                listings.push_back(waste);
                if (waste.getId() >= nextId) {
                    nextId = waste.getId() + 1;
                }
            }
        }
        inFile.close();

        // If file was empty, re-seed
        if (listings.empty()) {
            seedSampleData();
            saveToFile();
        }
    }

    // Save records using std::ofstream
    void saveToFile() const {
        std::ofstream outFile(dataFilePath, std::ios::trunc);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open " << dataFilePath << " for writing." << std::endl;
            return;
        }
        for (const auto& item : listings) {
            outFile << item.toFileString() << "\n";
        }
        outFile.close();
    }

    // Seed realistic sample agricultural by-products for demonstration
    void seedSampleData() {
        listings.clear();
        listings.push_back(AgriculturalWaste(101, "Rice Husk", "Husk & Shell", 5000.0, 3.50, "Punjab", "Dry & Clean", "Sardar Gurpreet Singh", "9876501234"));
        listings.push_back(AgriculturalWaste(102, "Sugarcane Bagasse", "Bagasse & Fibre", 8500.0, 2.80, "Maharashtra", "Semi-Dry (Baled)", "Ramesh Patil", "9823012345"));
        listings.push_back(AgriculturalWaste(103, "Coconut Husk", "Husk & Shell", 4200.0, 4.20, "Kerala", "Dry & Baled", "K. V. Narayanan", "9447012345"));
        listings.push_back(AgriculturalWaste(104, "Groundnut Shells", "Husk & Shell", 3000.0, 3.00, "Gujarat", "Crushed & Dry", "Bhavesh Patel", "9898012345"));
        listings.push_back(AgriculturalWaste(105, "Banana Pseudo-stem Fibre", "Bagasse & Fibre", 1800.0, 15.00, "Tamil Nadu", "Extracted Raw Fibre", "M. Murugan", "9789012345"));
        listings.push_back(AgriculturalWaste(106, "Wheat Straw", "Straw & Stalks", 6000.0, 2.20, "Haryana", "Baled & Dry", "Baljeet Sharma", "9812012345"));
        nextId = 107;
    }

    // Add listing and update static session count
    AgriculturalWaste addListing(const std::string& mat, const std::string& cat,
                                 double qty, double price, const std::string& loc,
                                 const std::string& cond, const std::string& fName,
                                 const std::string& fContact) {
        int assignedId = nextId++;
        AgriculturalWaste newWaste(assignedId, mat, cat, qty, price, loc, cond, fName, fContact);
        listings.push_back(newWaste);

        // Increment Static Data Member
        AgriculturalWaste::sessionRecordCount++;

        saveToFile();
        return newWaste;
    }

    // Delete record by ID
    bool deleteListing(int id) {
        auto it = std::remove_if(listings.begin(), listings.end(),
                                 [id](const AgriculturalWaste& w) { return w.getId() == id; });
        if (it != listings.end()) {
            listings.erase(it, listings.end());
            saveToFile();
            return true;
        }
        return false;
    }

    // Get all records
    const std::vector<AgriculturalWaste>& getAllListings() const {
        return listings;
    }

    // Summary Statistics
    int getTotalCount() const { return static_cast<int>(listings.size()); }

    double getTotalQuantity() const {
        double total = 0.0;
        for (const auto& w : listings) {
            total += w.getQuantityKg();
        }
        return total;
    }

    int getCategoryCount() const {
        std::vector<std::string> cats;
        for (const auto& w : listings) {
            if (std::find(cats.begin(), cats.end(), w.getCategory()) == cats.end()) {
                cats.push_back(w.getCategory());
            }
        }
        return static_cast<int>(cats.size());
    }

    // Search and Filter Function
    std::vector<AgriculturalWaste> searchListings(const std::string& matQuery,
                                                 const std::string& catFilter,
                                                 const std::string& locQuery,
                                                 double minQty) const {
        std::vector<AgriculturalWaste> results;
        std::string lowMat = toLower(matQuery);
        std::string lowCat = toLower(catFilter);
        std::string lowLoc = toLower(locQuery);

        for (const auto& item : listings) {
            bool matches = true;

            if (!matQuery.empty()) {
                std::string itemMat = toLower(item.getMaterialName());
                if (itemMat.find(lowMat) == std::string::npos) {
                    matches = false;
                }
            }

            if (!catFilter.empty() && catFilter != "all") {
                std::string itemCat = toLower(item.getCategory());
                if (itemCat != lowCat) {
                    matches = false;
                }
            }

            if (!locQuery.empty()) {
                std::string itemLoc = toLower(item.getLocation());
                if (itemLoc.find(lowLoc) == std::string::npos) {
                    matches = false;
                }
            }

            if (minQty > 0 && item.getQuantityKg() < minQty) {
                matches = false;
            }

            if (matches) {
                results.push_back(item);
            }
        }
        return results;
    }

    // Resource Matching Engine (Deterministic C++ Logic, No AI/ML)
    std::vector<MatchResult> matchSuppliers(const std::string& targetMat,
                                           double neededQty,
                                           const std::string& prefLoc,
                                           double maxBudget) const {
        std::vector<MatchResult> matched;
        std::string lowTarget = toLower(targetMat);
        std::string lowLoc = toLower(prefLoc);

        for (const auto& item : listings) {
            int score = 0;
            std::vector<std::string> reasons;
            std::string itemMat = toLower(item.getMaterialName());
            std::string itemLoc = toLower(item.getLocation());

            // 1. Material Matching (Up to 40 Points)
            if (!lowTarget.empty()) {
                if (itemMat == lowTarget) {
                    score += 40;
                    reasons.push_back("Exact Material Match: Listing offers '" + item.getMaterialName() + "' (+40%)");
                } else if (itemMat.find(lowTarget) != std::string::npos || lowTarget.find(itemMat) != std::string::npos) {
                    score += 25;
                    reasons.push_back("Partial Material Match: Related by-product category (+25%)");
                } else {
                    // If target material was specified and does not match at all, skip listing
                    continue;
                }
            } else {
                score += 20; // Default when no material filter is locked
                reasons.push_back("General Agro-Byproduct Supplier (+20%)");
            }

            // 2. Quantity Sufficiency (Up to 30 Points)
            if (neededQty > 0) {
                if (item.getQuantityKg() >= neededQty) {
                    score += 30;
                    std::ostringstream oss;
                    oss << "Quantity Surplus: Available " << std::fixed << std::setprecision(0)
                        << item.getQuantityKg() << " kg satisfies needed " << neededQty << " kg (+30%)";
                    reasons.push_back(oss.str());
                } else if (item.getQuantityKg() >= (neededQty * 0.5)) {
                    score += 15;
                    std::ostringstream oss;
                    oss << "Partial Lot Fulfillment: Available " << std::fixed << std::setprecision(0)
                        << item.getQuantityKg() << " kg covers over 50% of requirement (+15%)";
                    reasons.push_back(oss.str());
                } else {
                    reasons.push_back("Minor Batch: Available quantity is below 50% requirement (+0%)");
                }
            } else {
                score += 15;
                reasons.push_back("Standard Batch Quantity Available (+15%)");
            }

            // 3. Location Proximity (Up to 20 Points)
            if (!lowLoc.empty()) {
                if (itemLoc.find(lowLoc) != std::string::npos || lowLoc.find(itemLoc) != std::string::npos) {
                    score += 20;
                    reasons.push_back("Geographical Proximity: Farmer is located in " + item.getLocation() + " (Reduced transport emissions) (+20%)");
                } else {
                    reasons.push_back("Interstate Logistics: Supplier in " + item.getLocation() + " (+0%)");
                }
            } else {
                score += 10;
                reasons.push_back("Direct Regional Sourcing (+10%)");
            }

            // 4. Budget Feasibility (Up to 10 Points)
            if (maxBudget > 0) {
                if (item.getPricePerKg() <= maxBudget) {
                    score += 10;
                    std::ostringstream oss;
                    oss << "Budget Compliant: Asking price ₹" << std::fixed << std::setprecision(2)
                        << item.getPricePerKg() << "/kg is within budget cap of ₹" << maxBudget << "/kg (+10%)";
                    reasons.push_back(oss.str());
                } else {
                    std::ostringstream oss;
                    oss << "Price Cap Exceeded: Asking ₹" << std::fixed << std::setprecision(2)
                        << item.getPricePerKg() << "/kg vs budget ₹" << maxBudget << "/kg (Negotiable) (+0%)";
                    reasons.push_back(oss.str());
                }
            } else {
                score += 10;
                reasons.push_back("Direct Farm-Gate Price (+10%)");
            }

            // Cap score at 100
            if (score > 100) score = 100;

            std::string badgeClass = "score-low";
            if (score >= 80) badgeClass = "score-high";
            else if (score >= 50) badgeClass = "score-med";

            MatchResult res;
            res.listing = item;
            res.score = score;
            res.badgeClass = badgeClass;
            res.matchReasons = reasons;
            matched.push_back(res);
        }

        // Sort descending by match score
        std::sort(matched.begin(), matched.end(),
                  [](const MatchResult& a, const MatchResult& b) {
                      return a.score > b.score;
                  });

        return matched;
    }
};

// ============================================================================
// 3. UTILITY FUNCTIONS: URL DECODING & HTTP PARSING
// ============================================================================

// URL Decoder to convert percent-encoded characters and '+' into standard characters
std::string urlDecode(const std::string& str) {
    std::string result;
    result.reserve(str.length());
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            std::string hexStr = str.substr(i + 1, 2);
            char ch = static_cast<char>(std::strtol(hexStr.c_str(), nullptr, 16));
            result += ch;
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

// Parse form-urlencoded key-value pairs (from POST body or GET query)
std::map<std::string, std::string> parseFormData(const std::string& query) {
    std::map<std::string, std::string> data;
    std::stringstream ss(query);
    std::string pair;
    while (std::getline(ss, pair, '&')) {
        if (pair.empty()) continue;
        size_t eqPos = pair.find('=');
        if (eqPos != std::string::npos) {
            std::string key = urlDecode(pair.substr(0, eqPos));
            std::string val = urlDecode(pair.substr(eqPos + 1));
            data[key] = val;
        } else {
            data[urlDecode(pair)] = "";
        }
    }
    return data;
}

// Read entire file content (e.g. style.css)
std::string readFileContent(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// ============================================================================
// 4. HTML PAGE RENDERING ENGINE (Pure HTML5 & CSS3, Zero JavaScript)
// ============================================================================

std::string renderNavbar(const std::string& activePage) {
    std::ostringstream oss;
    oss << "  <header class=\"navbar\">\n"
        << "    <div class=\"nav-container\">\n"
        << "      <a href=\"/\" class=\"brand\">\n"
        << "        <span class=\"brand-icon\">🌱</span>\n"
        << "        <div>\n"
        << "          <span class=\"brand-title\">Smart Agri-Waste Marketplace</span>\n"
        << "          <span class=\"brand-subtitle\">Circular Bio-Economy Platform</span>\n"
        << "        </div>\n"
        << "      </a>\n"
        << "      <nav>\n"
        << "        <ul class=\"nav-links\">\n"
        << "          <li><a href=\"/\" class=\"nav-link " << (activePage == "home" ? "active" : "") << "\">Home</a></li>\n"
        << "          <li><a href=\"/register\" class=\"nav-link " << (activePage == "register" ? "active" : "") << "\">Farmer Portal</a></li>\n"
        << "          <li><a href=\"/listings\" class=\"nav-link " << (activePage == "listings" ? "active" : "") << "\">Available Materials</a></li>\n"
        << "          <li><a href=\"/search\" class=\"nav-link " << (activePage == "search" ? "active" : "") << "\">Buyer Search</a></li>\n"
        << "          <li><a href=\"/match\" class=\"nav-link btn-highlight " << (activePage == "match" ? "active" : "") << "\">Resource Matcher</a></li>\n"
        << "          <li><a href=\"/about\" class=\"nav-link " << (activePage == "about" ? "active" : "") << "\">About Project</a></li>\n"
        << "        </ul>\n"
        << "      </nav>\n"
        << "    </div>\n"
        << "  </header>\n";
    return oss.str();
}

std::string renderFooter() {
    std::ostringstream oss;
    oss << "  <footer class=\"footer\">\n"
        << "    <div class=\"footer-inner\">\n"
        << "      <div>\n"
        << "        <h4 class=\"footer-title\">🌱 Smart Agri-Waste Marketplace</h4>\n"
        << "        <p class=\"footer-desc\">\n"
        << "          An engineering solution to agricultural residue management. Transforming crop waste into valuable raw material for paper, energy, and sustainable packaging.\n"
        << "        </p>\n"
        << "      </div>\n"
        << "      <div>\n"
        << "        <h4 class=\"footer-title\">Platform Portals</h4>\n"
        << "        <ul class=\"footer-links\">\n"
        << "          <li><a href=\"/register\">Farmer Registration Form</a></li>\n"
        << "          <li><a href=\"/listings\">Available Materials Inventory</a></li>\n"
        << "          <li><a href=\"/search\">Industrial Buyer Search</a></li>\n"
        << "          <li><a href=\"/match\">Deterministic Resource Matcher</a></li>\n"
        << "        </ul>\n"
        << "      </div>\n"
        << "      <div>\n"
        << "        <h4 class=\"footer-title\">Academic Details</h4>\n"
        << "        <p class=\"footer-desc\">\n"
        << "          <strong>Course:</strong> Second-Year B.Tech / B.E. Computer Science & Engineering<br>\n"
        << "          <strong>Subject:</strong> Object-Oriented Programming with C++ & Web Mini Project<br>\n"
        << "          <strong>Tech Stack:</strong> HTML5, CSS3, C++ Sockets, fstream File I/O (No JavaScript)\n"
        << "        </p>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "    <div class=\"footer-bottom\">\n"
        << "      &copy; 2026 Smart Agricultural By-Product Marketplace • CSE Mini Project • Designed with Zero JavaScript\n"
        << "    </div>\n"
        << "  </footer>\n";
    return oss.str();
}

std::string getCategoryBadgeClass(const std::string& cat) {
    if (cat.find("Husk") != std::string::npos) return "badge-husk";
    if (cat.find("Bagasse") != std::string::npos) return "badge-bagasse";
    if (cat.find("Straw") != std::string::npos) return "badge-straw";
    if (cat.find("Fibre") != std::string::npos) return "badge-fibre";
    return "badge-oilseed";
}

// Page: Home
std::string renderHomePage(const WasteMarketplace& marketplace) {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>Smart Agricultural By-Product Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("home")
        << "  <section class=\"hero\">\n"
        << "    <div class=\"hero-inner\">\n"
        << "      <span class=\"hero-badge\">Second-Year CSE Mini Project • C++ Backend & OOP</span>\n"
        << "      <h1 class=\"hero-title\">Smart Agricultural By-Product Marketplace</h1>\n"
        << "      <p class=\"hero-tagline\">“Turning Agricultural By-Products into Valuable Resources.”</p>\n"
        << "      <p class=\"hero-description\">\n"
        << "        A decentralized platform directly bridging Indian farmers with industrial buyers to trade\n"
        << "        crop residues, husks, bagasse, and fibres. Mitigating crop burning, boosting rural income,\n"
        << "        and empowering sustainable manufacturing.\n"
        << "      </p>\n"
        << "      <div class=\"hero-actions\">\n"
        << "        <a href=\"/register\" class=\"btn btn-primary\">👨‍🌾 Farmer: List By-Product</a>\n"
        << "        <a href=\"/match\" class=\"btn btn-accent\">🏭 Buyer: Smart Resource Match</a>\n"
        << "        <a href=\"/listings\" class=\"btn btn-secondary\">📦 Browse All Materials</a>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "  </section>\n"
        << "  <div class=\"stats-container\">\n"
        << "    <div class=\"stats-grid\">\n"
        << "      <div class=\"stat-card\">\n"
        << "        <div class=\"stat-icon\">🌾</div>\n"
        << "        <div>\n"
        << "          <div class=\"stat-value\">" << marketplace.getTotalCount() << "</div>\n"
        << "          <div class=\"stat-label\">Listed Materials</div>\n"
        << "          <div class=\"stat-sub\">Active Batches in C++ Vector</div>\n"
        << "        </div>\n"
        << "      </div>\n"
        << "      <div class=\"stat-card\">\n"
        << "        <div class=\"stat-icon\">⚖️</div>\n"
        << "        <div>\n"
        << "          <div class=\"stat-value\">" << std::fixed << std::setprecision(0) << marketplace.getTotalQuantity() << " kg</div>\n"
        << "          <div class=\"stat-label\">Total Available Quantity</div>\n"
        << "          <div class=\"stat-sub\">Ready for Industrial Pickup</div>\n"
        << "        </div>\n"
        << "      </div>\n"
        << "      <div class=\"stat-card\">\n"
        << "        <div class=\"stat-icon\">🏷️</div>\n"
        << "        <div>\n"
        << "          <div class=\"stat-value\">" << marketplace.getCategoryCount() << "</div>\n"
        << "          <div class=\"stat-label\">Material Categories</div>\n"
        << "          <div class=\"stat-sub\">Husks, Bagasse, Straw & More</div>\n"
        << "        </div>\n"
        << "      </div>\n"
        << "      <div class=\"stat-card\">\n"
        << "        <div class=\"stat-icon\">⚡</div>\n"
        << "        <div>\n"
        << "          <div class=\"stat-value\">" << AgriculturalWaste::sessionRecordCount << "</div>\n"
        << "          <div class=\"stat-label\">Added This Session</div>\n"
        << "          <div class=\"stat-sub\">C++ Static Member Variable</div>\n"
        << "        </div>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "  </div>\n"
        << "  <main class=\"main-wrapper\">\n"
        << "    <section class=\"section-header\">\n"
        << "      <span class=\"section-subtitle\">Streamlined Workflow</span>\n"
        << "      <h2 class=\"section-title\">How the Marketplace Operates</h2>\n"
        << "      <p class=\"section-desc\">Connecting the agricultural supply chain with zero middleman commission using C++ backend processing.</p>\n"
        << "    </section>\n"
        << "    <div class=\"steps-grid\">\n"
        << "      <div class=\"step-card\">\n"
        << "        <div class=\"step-number\">01</div>\n"
        << "        <div class=\"step-icon\">👨‍🌾</div>\n"
        << "        <h3 class=\"step-title\">Farmer Registers Residue</h3>\n"
        << "        <p class=\"step-text\">Farmers list crop residues with quantity, price per kg, location, and condition using our lightweight, accessible HTML form.</p>\n"
        << "      </div>\n"
        << "      <div class=\"step-card\">\n"
        << "        <div class=\"step-number\">02</div>\n"
        << "        <div class=\"step-icon\">⚙️</div>\n"
        << "        <h3 class=\"step-title\">C++ File Persistence</h3>\n"
        << "        <p class=\"step-text\">Inputs are encapsulated in OOP objects and saved into persistent disk files via C++ fstream file handling.</p>\n"
        << "      </div>\n"
        << "      <div class=\"step-card\">\n"
        << "        <div class=\"step-number\">03</div>\n"
        << "        <div class=\"step-icon\">🏭</div>\n"
        << "        <h3 class=\"step-title\">Industrial Match & Reuse</h3>\n"
        << "        <p class=\"step-text\">Paper mills, biofuel plants, and recyclers find and connect with matching suppliers via deterministic C++ conditions.</p>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "    <div class=\"portal-grid\">\n"
        << "      <div class=\"portal-card farmer-portal\">\n"
        << "        <div>\n"
        << "          <span class=\"portal-badge badge-bagasse\">🌾 For Farmers & Cooperatives</span>\n"
        << "          <h3 class=\"portal-title\">Monetize Your Harvest Residues</h3>\n"
        << "          <p class=\"portal-desc\">Stop burning crop residue. Turn stalks, husks, and shells into an extra source of income with verified industrial buyers.</p>\n"
        << "          <ul class=\"portal-features\">\n"
        << "            <li>✅ Free listing with direct contact details</li>\n"
        << "            <li>✅ Fair market pricing per kilogram</li>\n"
        << "            <li>✅ Bulk collection directly from farm gate</li>\n"
        << "            <li>✅ Prevents air pollution & legal stubble penalties</li>\n"
        << "          </ul>\n"
        << "        </div>\n"
        << "        <a href=\"/register\" class=\"btn btn-primary\">Go to Farmer Registration →</a>\n"
        << "      </div>\n"
        << "      <div class=\"portal-card buyer-portal\">\n"
        << "        <div>\n"
        << "          <span class=\"portal-badge badge-husk\">🏭 For Industrial Buyers</span>\n"
        << "          <h3 class=\"portal-title\">Procure Sustainable Raw Biomass</h3>\n"
        << "          <p class=\"portal-desc\">Source high-volume agricultural by-products for biofuel pellets, packaging, animal feed, paper, and thermal boilers.</p>\n"
        << "          <ul class=\"portal-features\">\n"
        << "            <li>🔍 Search by material name, category, and state</li>\n"
        << "            <li>⚡ Smart scoring algorithm ranks matching suppliers</li>\n"
        << "            <li>📊 View verified quantity and condition specifications</li>\n"
        << "            <li>📞 Direct telephone connect with local farmers</li>\n"
        << "          </ul>\n"
        << "        </div>\n"
        << "        <a href=\"/search\" class=\"btn btn-accent\">Search & Match Suppliers →</a>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "    <section class=\"section-header left-aligned\">\n"
        << "      <span class=\"section-subtitle\">Recent Additions</span>\n"
        << "      <h2 class=\"section-title\">Available By-Products in Market</h2>\n"
        << "    </section>\n"
        << "    <div class=\"listings-grid\">\n";

    const auto& all = marketplace.getAllListings();
    int count = 0;
    for (auto it = all.rbegin(); it != all.rend() && count < 3; ++it, ++count) {
        oss << "      <div class=\"item-card\">\n"
            << "        <div class=\"item-card-header\">\n"
            << "          <h3 class=\"item-title\">" << it->getMaterialName() << "</h3>\n"
            << "          <span class=\"badge " << getCategoryBadgeClass(it->getCategory()) << "\">" << it->getCategory() << "</span>\n"
            << "        </div>\n"
            << "        <div class=\"item-card-body\">\n"
            << "          <div class=\"spec-list\">\n"
            << "            <div class=\"spec-item\"><span class=\"spec-label\">Available Qty</span><span class=\"spec-value\">" << std::fixed << std::setprecision(0) << it->getQuantityKg() << " kg</span></div>\n"
            << "            <div class=\"spec-item\"><span class=\"spec-label\">Price / Kg</span><span class=\"spec-value price\">₹" << std::fixed << std::setprecision(2) << it->getPricePerKg() << "</span></div>\n"
            << "            <div class=\"spec-item\"><span class=\"spec-label\">Location</span><span class=\"spec-value\">" << it->getLocation() << "</span></div>\n"
            << "            <div class=\"spec-item\"><span class=\"spec-label\">Condition</span><span class=\"spec-value\">" << it->getCondition() << "</span></div>\n"
            << "          </div>\n"
            << "          <div class=\"supplier-box\"><strong>Farmer: " << it->getFarmerName() << "</strong><span>📞 " << it->getFarmerContact() << "</span></div>\n"
            << "        </div>\n"
            << "        <div class=\"item-card-footer\">\n"
            << "          <span style=\"font-size:0.8rem; color:var(--text-muted);\">Lot Est: ₹" << std::fixed << std::setprecision(0) << it->calculateTotalValue() << "</span>\n"
            << "          <a href=\"/listings\" class=\"btn btn-secondary btn-sm\">View Details</a>\n"
            << "        </div>\n"
            << "      </div>\n";
    }

    oss << "    </div>\n"
        << "    <div style=\"text-align: center; margin: 1.5rem 0 3rem;\">\n"
        << "      <a href=\"/listings\" class=\"btn btn-primary\">Browse Full Inventory (" << marketplace.getTotalCount() << " Batches) →</a>\n"
        << "    </div>\n"
        << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// Page: Farmer Registration Form
std::string renderRegisterPage(const std::string& successMsg = "") {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>Farmer Registration | Smart Agri-Waste Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("register")
        << "  <main class=\"main-wrapper\">\n";

    if (!successMsg.empty()) {
        oss << "    <div class=\"alert alert-success\">\n"
            << "      <span class=\"alert-icon\">✅</span>\n"
            << "      <div class=\"alert-content\">\n"
            << "        <h4>Registration Successful!</h4>\n"
            << "        <p>" << successMsg << "</p>\n"
            << "      </div>\n"
            << "    </div>\n";
    }

    oss << "    <div class=\"section-header\">\n"
        << "      <span class=\"section-subtitle\">Farmer Portal</span>\n"
        << "      <h1 class=\"section-title\">Register Agricultural By-Product</h1>\n"
        << "      <p class=\"section-desc\">List your harvest residues directly into our C++ backend. Connect with verified industrial recyclers and prevent crop burning.</p>\n"
        << "    </div>\n"
        << "    <div class=\"form-card\">\n"
        << "      <form method=\"POST\" action=\"/register\">\n"
        << "        <div class=\"form-grid\">\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"farmerName\">Farmer Full Name <span class=\"req\">*</span></label>\n"
        << "            <input type=\"text\" id=\"farmerName\" name=\"farmerName\" class=\"form-control\" placeholder=\"e.g., Ramesh Patil\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"farmerContact\">Contact / Mobile Number <span class=\"req\">*</span></label>\n"
        << "            <input type=\"tel\" id=\"farmerContact\" name=\"farmerContact\" class=\"form-control\" placeholder=\"e.g., 9823012345\" required pattern=\"[0-9]{10}\" title=\"Please enter a 10-digit mobile number\">\n"
        << "            <span class=\"form-hint\">10-digit primary mobile contact</span>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"materialName\">By-Product / Material Name <span class=\"req\">*</span></label>\n"
        << "            <input type=\"text\" id=\"materialName\" name=\"materialName\" class=\"form-control\" placeholder=\"e.g., Sugarcane Bagasse, Rice Husk\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"category\">Residue Category <span class=\"req\">*</span></label>\n"
        << "            <select id=\"category\" name=\"category\" class=\"form-control\" required>\n"
        << "              <option value=\"\">-- Select Category --</option>\n"
        << "              <option value=\"Husk & Shell\">Husk & Shell (Rice, Coconut, Groundnut)</option>\n"
        << "              <option value=\"Bagasse & Fibre\">Bagasse & Fibre (Sugarcane, Banana)</option>\n"
        << "              <option value=\"Straw & Stalks\">Straw & Stalks (Wheat, Paddy, Mustard)</option>\n"
        << "              <option value=\"Oilseed Waste\">Oilseed Waste / Cake</option>\n"
        << "              <option value=\"Leaves & Fronds\">Leaves, Fronds & Stems</option>\n"
        << "            </select>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"quantityKg\">Quantity (Kilograms) <span class=\"req\">*</span></label>\n"
        << "            <input type=\"number\" id=\"quantityKg\" name=\"quantityKg\" class=\"form-control\" placeholder=\"e.g., 4000\" min=\"10\" step=\"any\" required>\n"
        << "            <span class=\"form-hint\">Minimum bulk batch: 10 kg</span>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"pricePerKg\">Asking Price (₹ per Kg) <span class=\"req\">*</span></label>\n"
        << "            <input type=\"number\" id=\"pricePerKg\" name=\"pricePerKg\" class=\"form-control\" placeholder=\"e.g., 3.50\" min=\"0.1\" step=\"0.01\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"location\">Farm Location / State <span class=\"req\">*</span></label>\n"
        << "            <input type=\"text\" id=\"location\" name=\"location\" class=\"form-control\" placeholder=\"e.g., Punjab, Kolhapur (Maharashtra)\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"condition\">Material Condition <span class=\"req\">*</span></label>\n"
        << "            <select id=\"condition\" name=\"condition\" class=\"form-control\" required>\n"
        << "              <option value=\"Dry & Clean\">Dry & Clean (Moisture &lt; 10%)</option>\n"
        << "              <option value=\"Semi-Dry (Baled)\">Semi-Dry (Baled)</option>\n"
        << "              <option value=\"Loose / Raw\">Loose / Raw Field Stubble</option>\n"
        << "              <option value=\"Crushed / Powder\">Crushed / Granulated / Powder</option>\n"
        << "              <option value=\"Extracted Raw Fibre\">Extracted Raw Fibre</option>\n"
        << "            </select>\n"
        << "          </div>\n"
        << "        </div>\n"
        << "        <div style=\"text-align: right; margin-top: 1rem;\">\n"
        << "          <button type=\"submit\" class=\"btn btn-primary\" style=\"padding: 0.85rem 2rem;\">Submit & Register Material</button>\n"
        << "        </div>\n"
        << "      </form>\n"
        << "    </div>\n"
        << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// Page: Available Materials (Cards + Table + Deletion Form)
std::string renderListingsPage(const WasteMarketplace& marketplace, const std::string& actionMsg = "") {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>Available Materials | Smart Agri-Waste Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("listings")
        << "  <main class=\"main-wrapper\">\n";

    if (!actionMsg.empty()) {
        oss << "    <div class=\"alert alert-success\">\n"
            << "      <span class=\"alert-icon\">ℹ️</span>\n"
            << "      <div class=\"alert-content\"><h4>Status Notification</h4><p>" << actionMsg << "</p></div>\n"
            << "    </div>\n";
    }

    oss << "    <div class=\"section-header left-aligned\">\n"
        << "      <span class=\"section-subtitle\">Master Inventory</span>\n"
        << "      <h1 class=\"section-title\">Available Agricultural By-Products</h1>\n"
        << "      <p class=\"section-desc\">Live records loaded dynamically from the C++ backend and persistent text database.</p>\n"
        << "    </div>\n"
        << "    <div class=\"listings-grid\">\n";

    const auto& items = marketplace.getAllListings();
    if (items.empty()) {
        oss << "      <p style=\"grid-column: 1 / -1; text-align: center; color: var(--text-muted); padding: 2rem;\">No materials listed yet. Register the first batch!</p>\n";
    } else {
        for (const auto& item : items) {
            oss << "      <div class=\"item-card\">\n"
                << "        <div class=\"item-card-header\">\n"
                << "          <h3 class=\"item-title\">" << item.getMaterialName() << "</h3>\n"
                << "          <span class=\"badge " << getCategoryBadgeClass(item.getCategory()) << "\">" << item.getCategory() << "</span>\n"
                << "        </div>\n"
                << "        <div class=\"item-card-body\">\n"
                << "          <div class=\"spec-list\">\n"
                << "            <div class=\"spec-item\"><span class=\"spec-label\">Batch ID</span><span class=\"spec-value\">#" << item.getId() << "</span></div>\n"
                << "            <div class=\"spec-item\"><span class=\"spec-label\">Price / Kg</span><span class=\"spec-value price\">₹" << std::fixed << std::setprecision(2) << item.getPricePerKg() << "</span></div>\n"
                << "            <div class=\"spec-item\"><span class=\"spec-label\">Available Qty</span><span class=\"spec-value\">" << std::fixed << std::setprecision(0) << item.getQuantityKg() << " kg</span></div>\n"
                << "            <div class=\"spec-item\"><span class=\"spec-label\">Location</span><span class=\"spec-value\">" << item.getLocation() << "</span></div>\n"
                << "            <div class=\"spec-item\" style=\"grid-column: 1 / -1;\"><span class=\"spec-label\">Material Condition</span><span class=\"spec-value\">" << item.getCondition() << "</span></div>\n"
                << "          </div>\n"
                << "          <div class=\"supplier-box\"><strong>Supplier: " << item.getFarmerName() << "</strong><span>📞 " << item.getFarmerContact() << "</span></div>\n"
                << "        </div>\n"
                << "        <div class=\"item-card-footer\">\n"
                << "          <span style=\"font-size:0.85rem; font-weight:700; color:var(--primary-dark);\">Total: ₹" << std::fixed << std::setprecision(0) << item.calculateTotalValue() << "</span>\n"
                << "          <form method=\"POST\" action=\"/delete\" style=\"display:inline;\">\n"
                << "            <input type=\"hidden\" name=\"id\" value=\"" << item.getId() << "\">\n"
                << "            <button type=\"submit\" class=\"btn btn-danger btn-sm\">Delete</button>\n"
                << "          </form>\n"
                << "        </div>\n"
                << "      </div>\n";
        }
    }

    oss << "    </div>\n"
        << "    <div class=\"section-header left-aligned\" style=\"margin-top: 2rem;\">\n"
        << "      <span class=\"section-subtitle\">Tabular Audit View</span>\n"
        << "      <h2 class=\"section-title\">Comprehensive Records Table</h2>\n"
        << "    </div>\n"
        << "    <div class=\"table-container\">\n"
        << "      <table class=\"custom-table\">\n"
        << "        <thead>\n"
        << "          <tr>\n"
        << "            <th>ID</th>\n"
        << "            <th>Material</th>\n"
        << "            <th>Category</th>\n"
        << "            <th>Quantity (kg)</th>\n"
        << "            <th>Price/kg</th>\n"
        << "            <th>Total Value</th>\n"
        << "            <th>Location</th>\n"
        << "            <th>Condition</th>\n"
        << "            <th>Farmer Name</th>\n"
        << "            <th>Contact</th>\n"
        << "            <th>Action</th>\n"
        << "          </tr>\n"
        << "        </thead>\n"
        << "        <tbody>\n";

    for (const auto& item : items) {
        oss << "          <tr>\n"
            << "            <td><strong>#" << item.getId() << "</strong></td>\n"
            << "            <td>" << item.getMaterialName() << "</td>\n"
            << "            <td><span class=\"badge " << getCategoryBadgeClass(item.getCategory()) << "\">" << item.getCategory() << "</span></td>\n"
            << "            <td>" << std::fixed << std::setprecision(0) << item.getQuantityKg() << " kg</td>\n"
            << "            <td>₹" << std::fixed << std::setprecision(2) << item.getPricePerKg() << "</td>\n"
            << "            <td>₹" << std::fixed << std::setprecision(0) << item.calculateTotalValue() << "</td>\n"
            << "            <td>" << item.getLocation() << "</td>\n"
            << "            <td>" << item.getCondition() << "</td>\n"
            << "            <td>" << item.getFarmerName() << "</td>\n"
            << "            <td>" << item.getFarmerContact() << "</td>\n"
            << "            <td>\n"
            << "              <form method=\"POST\" action=\"/delete\" style=\"display:inline;\">\n"
            << "                <input type=\"hidden\" name=\"id\" value=\"" << item.getId() << "\">\n"
            << "                <button type=\"submit\" class=\"btn btn-danger btn-sm\">Delete</button>\n"
            << "              </form>\n"
            << "            </td>\n"
            << "          </tr>\n";
    }

    oss << "        </tbody>\n"
        << "      </table>\n"
        << "    </div>\n"
        << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// Page: Buyer Search & Filter
std::string renderSearchPage(const WasteMarketplace& marketplace,
                             const std::string& queryMat = "",
                             const std::string& queryCat = "",
                             const std::string& queryLoc = "",
                             double minQty = 0.0,
                             bool hasSearched = false) {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>Buyer Search | Smart Agri-Waste Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("search")
        << "  <main class=\"main-wrapper\">\n"
        << "    <div class=\"section-header\">\n"
        << "      <span class=\"section-subtitle\">Industrial Procurement</span>\n"
        << "      <h1 class=\"section-title\">Search Available Agricultural Materials</h1>\n"
        << "      <p class=\"section-desc\">Filter by by-product name, category, required volume, and location to connect directly with agricultural suppliers.</p>\n"
        << "    </div>\n"
        << "    <div class=\"form-card\" style=\"margin-bottom: 2.5rem;\">\n"
        << "      <form method=\"GET\" action=\"/search\">\n"
        << "        <div class=\"form-grid\">\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"material\">Material Name</label>\n"
        << "            <input type=\"text\" id=\"material\" name=\"material\" class=\"form-control\" placeholder=\"e.g., Husk, Bagasse, Straw\" value=\"" << queryMat << "\">\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"category\">Category Filter</label>\n"
        << "            <select id=\"category\" name=\"category\" class=\"form-control\">\n"
        << "              <option value=\"all\" " << (queryCat == "all" || queryCat.empty() ? "selected" : "") << ">All Categories</option>\n"
        << "              <option value=\"Husk & Shell\" " << (queryCat == "Husk & Shell" ? "selected" : "") << ">Husk & Shell</option>\n"
        << "              <option value=\"Bagasse & Fibre\" " << (queryCat == "Bagasse & Fibre" ? "selected" : "") << ">Bagasse & Fibre</option>\n"
        << "              <option value=\"Straw & Stalks\" " << (queryCat == "Straw & Stalks" ? "selected" : "") << ">Straw & Stalks</option>\n"
        << "              <option value=\"Oilseed Waste\" " << (queryCat == "Oilseed Waste" ? "selected" : "") << ">Oilseed Waste</option>\n"
        << "            </select>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"location\">Preferred Location / State</label>\n"
        << "            <input type=\"text\" id=\"location\" name=\"location\" class=\"form-control\" placeholder=\"e.g., Punjab, Maharashtra\" value=\"" << queryLoc << "\">\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"minQty\">Min Required Quantity (kg)</label>\n"
        << "            <input type=\"number\" id=\"minQty\" name=\"minQty\" class=\"form-control\" placeholder=\"e.g., 2000\" min=\"0\" value=\"" << (minQty > 0 ? std::to_string(static_cast<int>(minQty)) : "") << "\">\n"
        << "          </div>\n"
        << "        </div>\n"
        << "        <div style=\"display:flex; justify-content: flex-end; gap: 1rem;\">\n"
        << "          <a href=\"/search\" class=\"btn btn-secondary\">Reset Filters</a>\n"
        << "          <button type=\"submit\" class=\"btn btn-primary\">Filter & Search Suppliers</button>\n"
        << "        </div>\n"
        << "      </form>\n"
        << "    </div>\n";

    if (hasSearched) {
        auto results = marketplace.searchListings(queryMat, queryCat, queryLoc, minQty);
        oss << "    <div class=\"section-header left-aligned\">\n"
            << "      <span class=\"section-subtitle\">Query Output</span>\n"
            << "      <h2 class=\"section-title\">Matching Suppliers (" << results.size() << " Found)</h2>\n"
            << "    </div>\n";

        if (results.empty()) {
            oss << "    <div class=\"alert alert-warning\">\n"
                << "      <span class=\"alert-icon\">⚠️</span>\n"
                << "      <div class=\"alert-content\">\n"
                << "        <h4>No Matching Suppliers Found</h4>\n"
                << "        <p>No agricultural by-product matches all the specified filters. Try loosening your search criteria, reducing minimum quantity, or check back as more farmers list materials.</p>\n"
                << "      </div>\n"
                << "    </div>\n";
        } else {
            oss << "    <div class=\"listings-grid\">\n";
            for (const auto& item : results) {
                oss << "      <div class=\"item-card\">\n"
                    << "        <div class=\"item-card-header\">\n"
                    << "          <h3 class=\"item-title\">" << item.getMaterialName() << "</h3>\n"
                    << "          <span class=\"badge " << getCategoryBadgeClass(item.getCategory()) << "\">" << item.getCategory() << "</span>\n"
                    << "        </div>\n"
                    << "        <div class=\"item-card-body\">\n"
                    << "          <div class=\"spec-list\">\n"
                    << "            <div class=\"spec-item\"><span class=\"spec-label\">Available Qty</span><span class=\"spec-value\">" << std::fixed << std::setprecision(0) << item.getQuantityKg() << " kg</span></div>\n"
                    << "            <div class=\"spec-item\"><span class=\"spec-label\">Asking Price</span><span class=\"spec-value price\">₹" << std::fixed << std::setprecision(2) << item.getPricePerKg() << "/kg</span></div>\n"
                    << "            <div class=\"spec-item\"><span class=\"spec-label\">Location</span><span class=\"spec-value\">" << item.getLocation() << "</span></div>\n"
                    << "            <div class=\"spec-item\"><span class=\"spec-label\">Condition</span><span class=\"spec-value\">" << item.getCondition() << "</span></div>\n"
                    << "          </div>\n"
                    << "          <div class=\"supplier-box\"><strong>Supplier: " << item.getFarmerName() << "</strong><span>📞 " << item.getFarmerContact() << "</span></div>\n"
                    << "        </div>\n"
                    << "        <div class=\"item-card-footer\">\n"
                    << "          <span style=\"font-size:0.85rem; color:var(--text-muted);\">Lot Total: ₹" << std::fixed << std::setprecision(0) << item.calculateTotalValue() << "</span>\n"
                    << "          <a href=\"tel:" << item.getFarmerContact() << "\" class=\"btn btn-primary btn-sm\">Call Farmer</a>\n"
                    << "        </div>\n"
                    << "      </div>\n";
            }
            oss << "    </div>\n";
        }
    }

    oss << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// Page: Intelligent Resource Matcher (Scored Recommendations + Explanations)
std::string renderMatchPage(const WasteMarketplace& marketplace,
                            const std::string& targetMat = "",
                            double neededQty = 0.0,
                            const std::string& prefLoc = "",
                            double maxBudget = 0.0,
                            bool hasSubmitted = false) {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>Smart Resource Matcher | Smart Agri-Waste Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("match")
        << "  <main class=\"main-wrapper\">\n"
        << "    <div class=\"section-header\">\n"
        << "      <span class=\"section-subtitle\">Algorithmic Matching Engine</span>\n"
        << "      <h1 class=\"section-title\">Intelligent Resource Matcher</h1>\n"
        << "      <p class=\"section-desc\">Evaluates material type, batch sufficiency, proximity logistics, and unit pricing using deterministic C++ conditions.</p>\n"
        << "    </div>\n"
        << "    <div class=\"form-card\">\n"
        << "      <form method=\"POST\" action=\"/match\">\n"
        << "        <div class=\"form-grid\">\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"targetMaterial\">Requested By-Product <span class=\"req\">*</span></label>\n"
        << "            <input type=\"text\" id=\"targetMaterial\" name=\"targetMaterial\" class=\"form-control\" placeholder=\"e.g., Rice Husk, Bagasse, Coconut Husk\" value=\"" << targetMat << "\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"requiredQuantity\">Required Volume (kg) <span class=\"req\">*</span></label>\n"
        << "            <input type=\"number\" id=\"requiredQuantity\" name=\"requiredQuantity\" class=\"form-control\" placeholder=\"e.g., 3000\" min=\"1\" value=\"" << (neededQty > 0 ? std::to_string(static_cast<int>(neededQty)) : "") << "\" required>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"preferredLocation\">Preferred Supplier Location</label>\n"
        << "            <input type=\"text\" id=\"preferredLocation\" name=\"preferredLocation\" class=\"form-control\" placeholder=\"e.g., Punjab, Kerala, Maharashtra\" value=\"" << prefLoc << "\">\n"
        << "            <span class=\"form-hint\">Proximity reduces transport costs & footprint</span>\n"
        << "          </div>\n"
        << "          <div class=\"form-group\">\n"
        << "            <label class=\"form-label\" for=\"maxBudget\">Max Budget Ceiling (₹ per Kg)</label>\n"
        << "            <input type=\"number\" id=\"maxBudget\" name=\"maxBudget\" class=\"form-control\" placeholder=\"e.g., 4.50\" min=\"0.1\" step=\"0.01\" value=\"" << (maxBudget > 0 ? std::to_string(maxBudget).substr(0,4) : "") << "\">\n"
        << "          </div>\n"
        << "        </div>\n"
        << "        <div style=\"text-align: right; margin-top: 1rem;\">\n"
        << "          <button type=\"submit\" class=\"btn btn-primary\" style=\"padding: 0.85rem 2rem;\">Execute C++ Resource Matcher</button>\n"
        << "        </div>\n"
        << "      </form>\n"
        << "    </div>\n";

    if (hasSubmitted) {
        auto matches = marketplace.matchSuppliers(targetMat, neededQty, prefLoc, maxBudget);
        oss << "    <div class=\"section-header left-aligned\">\n"
            << "      <span class=\"section-subtitle\">Engine Results</span>\n"
            << "      <h2 class=\"section-title\">Ranked Supplier Recommendations</h2>\n"
            << "    </div>\n";

        if (matches.empty()) {
            oss << "    <div class=\"alert alert-warning\">\n"
                << "      <span class=\"alert-icon\">⚠️</span>\n"
                << "      <div class=\"alert-content\">\n"
                << "        <h4>No Feasible Matches Found</h4>\n"
                << "        <p>No registered suppliers currently match the material '" << targetMat << "'. Consider checking alternative fibrous residues or notifying agricultural cooperatives.</p>\n"
                << "      </div>\n"
                << "    </div>\n";
        } else {
            for (const auto& m : matches) {
                oss << "    <div class=\"match-card\">\n"
                    << "      <div class=\"match-header\">\n"
                    << "        <div>\n"
                    << "          <h3 style=\"font-size:1.35rem; color:var(--primary-dark);\">" << m.listing.getMaterialName() << " • Batch #" << m.listing.getId() << "</h3>\n"
                    << "          <span style=\"font-size:0.88rem; color:var(--text-muted);\">Supplier: " << m.listing.getFarmerName() << " (" << m.listing.getLocation() << ")</span>\n"
                    << "        </div>\n"
                    << "        <span class=\"score-badge " << m.badgeClass << "\">Compatibility: " << m.score << "%</span>\n"
                    << "      </div>\n"
                    << "      <div class=\"spec-list\" style=\"margin-top:0.75rem;\">\n"
                    << "        <div class=\"spec-item\"><span class=\"spec-label\">Available Inventory</span><span class=\"spec-value\">" << std::fixed << std::setprecision(0) << m.listing.getQuantityKg() << " kg</span></div>\n"
                    << "        <div class=\"spec-item\"><span class=\"spec-label\">Offered Price</span><span class=\"spec-value price\">₹" << std::fixed << std::setprecision(2) << m.listing.getPricePerKg() << "/kg</span></div>\n"
                    << "        <div class=\"spec-item\"><span class=\"spec-label\">Material Condition</span><span class=\"spec-value\">" << m.listing.getCondition() << "</span></div>\n"
                    << "        <div class=\"spec-item\"><span class=\"spec-label\">Direct Contact</span><span class=\"spec-value\">📞 " << m.listing.getFarmerContact() << "</span></div>\n"
                    << "      </div>\n"
                    << "      <div class=\"match-reasons\">\n"
                    << "        <strong style=\"color:var(--primary-dark); font-size:0.85rem;\">Algorithm Match Rationale:</strong>\n"
                    << "        <ul>\n";

                for (const auto& r : m.matchReasons) {
                    oss << "          <li>• " << r << "</li>\n";
                }

                oss << "        </ul>\n"
                    << "      </div>\n"
                    << "    </div>\n";
            }
        }
    }

    oss << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// Page: About Project
std::string renderAboutPage() {
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
        << "  <meta charset=\"UTF-8\">\n"
        << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        << "  <title>About Project | Smart Agri-Waste Marketplace</title>\n"
        << "  <link rel=\"stylesheet\" href=\"/style.css\">\n"
        << "</head>\n<body>\n"
        << renderNavbar("about")
        << "  <main class=\"main-wrapper\">\n"
        << "    <div class=\"section-header\">\n"
        << "      <span class=\"section-subtitle\">Academic Documentation</span>\n"
        << "      <h1 class=\"section-title\">Smart Agricultural By-Product Marketplace</h1>\n"
        << "      <p class=\"section-desc\">A comprehensive second-year Computer Science Engineering mini project demonstrating Object-Oriented C++, system sockets, and file persistence.</p>\n"
        << "    </div>\n"
        << "    <div class=\"about-grid\">\n"
        << "      <div class=\"about-card\">\n"
        << "        <h3>⚠️ The Real-World Problem</h3>\n"
        << "        <p>Across India and developing agrarian nations, over 100 million tonnes of crop residue (paddy straw, sugarcane bagasse, groundnut shells) are incinerated annually. This causes severe seasonal smog, respiratory crises, and loss of valuable soil organic carbon.</p>\n"
        << "      </div>\n"
        << "      <div class=\"about-card\">\n"
        << "        <h3>🎯 Project Objectives</h3>\n"
        << "        <p>Provide a transparent, direct bridge between farmers and industrial consumers. Monetize crop residues as secondary income for agrarian families while supplying eco-friendly raw biomass for paper, packaging, bioplastics, and clean bio-energy boilers.</p>\n"
        << "      </div>\n"
        << "      <div class=\"about-card\">\n"
        << "        <h3>🌿 Environmental & Economic Impact</h3>\n"
        << "        <p>Promotes a circular bioeconomy: reduces CO2 and PM2.5 stubble emissions, prevents soil nutrient depletion, eliminates intermediary middlemen exploitation, and supports UN Sustainable Development Goals 12 (Responsible Consumption) and 13 (Climate Action).</p>\n"
        << "      </div>\n"
        << "      <div class=\"about-card\">\n"
        << "        <h3>🚀 Future Scope</h3>\n"
        << "        <p>Future engineering milestones include integrating GPS-based automated rural transport dispatch, escrow micro-payments for secure escrow settlement, IoT-enabled moisture verification sensors, and vernacular voice interfaces for rural farmers.</p>\n"
        << "      </div>\n"
        << "    </div>\n"
        << "    <div class=\"tech-spec-box\">\n"
        << "      <h3 style=\"color:var(--primary-dark); margin-bottom: 0.75rem;\">💻 Technical Architecture & OOP Specification</h3>\n"
        << "      <p style=\"font-size:0.95rem; line-height:1.7;\">\n"
        << "        <strong>1. Encapsulation:</strong> Private state attributes in <code>AgriculturalWaste</code>, <code>Farmer</code>, and <code>IndustrialBuyer</code> with public accessor methods.<br>\n"
        << "        <strong>2. Inheritance:</strong> Abstract base class <code>User</code> inherited by <code>Farmer</code> and <code>IndustrialBuyer</code>.<br>\n"
        << "        <strong>3. Polymorphism:</strong> Pure virtual member function <code>virtual std::string getRole() const = 0</code> overridden across child classes.<br>\n"
        << "        <strong>4. Static Members:</strong> <code>sessionRecordCount</code> tracks batches registered during current server runtime without global variables.<br>\n"
        << "        <strong>5. File I/O:</strong> Persistent pipe-delimited records serialized and parsed using C++ <code>std::ifstream</code> and <code>std::ofstream</code>.<br>\n"
        << "        <strong>6. Zero JavaScript Architecture:</strong> 100% native HTML5 form submissions processed synchronously by C++ socket routines.\n"
        << "      </p>\n"
        << "    </div>\n"
        << "  </main>\n"
        << renderFooter()
        << "</body>\n</html>";
    return oss.str();
}

// ============================================================================
// 5. C++ HTTP SERVER & REQUEST DISPATCHER
// ============================================================================

void sendHttpResponse(SOCKET clientSocket, const std::string& status, const std::string& contentType, const std::string& body) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n"
             << "Content-Type: " << contentType << "; charset=UTF-8\r\n"
             << "Content-Length: " << body.length() << "\r\n"
             << "Connection: close\r\n\r\n"
             << body;

    std::string responseStr = response.str();
    send(clientSocket, responseStr.c_str(), static_cast<int>(responseStr.length()), 0);
}

void handleClient(SOCKET clientSocket, WasteMarketplace& marketplace) {
    char buffer[8192];
    std::memset(buffer, 0, sizeof(buffer));

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        CLOSE_SOCKET(clientSocket);
        return;
    }

    std::string requestStr(buffer, bytesReceived);
    std::istringstream requestStream(requestStr);
    std::string method, path, protocol;
    requestStream >> method >> path >> protocol;

    // Separate Path and Query String
    std::string rawQuery = "";
    size_t qPos = path.find('?');
    if (qPos != std::string::npos) {
        rawQuery = path.substr(qPos + 1);
        path = path.substr(0, qPos);
    }

    // Normalize Absolute URL from Reverse Proxies (e.g. http://proxy/path)
    if (path.rfind("http://", 0) == 0) {
        size_t slashPos = path.find('/', 7);
        path = (slashPos != std::string::npos) ? path.substr(slashPos) : "/";
    } else if (path.rfind("https://", 0) == 0) {
        size_t slashPos = path.find('/', 8);
        path = (slashPos != std::string::npos) ? path.substr(slashPos) : "/";
    }

    // Extract POST Body (if applicable)
    std::string body = "";
    size_t headerEnd = requestStr.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        body = requestStr.substr(headerEnd + 4);
    }

    // ================= Route Dispatcher =================

    // 1. Static CSS File
    if (path == "/style.css") {
        std::string css = readFileContent("style.css");
        if (!css.empty()) {
            sendHttpResponse(clientSocket, "200 OK", "text/css", css);
        } else {
            sendHttpResponse(clientSocket, "404 Not Found", "text/plain", "style.css not found");
        }
    }
    // 2. Home Page
    else if (path == "/" || path == "/home") {
        std::string html = renderHomePage(marketplace);
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 3. Farmer Registration (GET Form)
    else if (path == "/register" && method == "GET") {
        std::string html = renderRegisterPage();
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 4. Farmer Registration (POST Submission)
    else if (path == "/register" && method == "POST") {
        auto formData = parseFormData(body);
        std::string fName = formData["farmerName"];
        std::string fContact = formData["farmerContact"];
        std::string mat = formData["materialName"];
        std::string cat = formData["category"];
        std::string loc = formData["location"];
        std::string cond = formData["condition"];
        double qty = 0.0;
        double price = 0.0;
        try {
            if (!formData["quantityKg"].empty()) qty = std::stod(formData["quantityKg"]);
            if (!formData["pricePerKg"].empty()) price = std::stod(formData["pricePerKg"]);
        } catch (...) {
            qty = 0.0;
            price = 0.0;
        }

        if (!fName.empty() && !mat.empty() && qty > 0 && price > 0) {
            auto added = marketplace.addListing(mat, cat, qty, price, loc, cond, fName, fContact);
            std::ostringstream msg;
            msg << "Batch #" << added.getId() << " ('" << added.getMaterialName()
                << "', " << std::fixed << std::setprecision(0) << added.getQuantityKg()
                << " kg @ ₹" << std::fixed << std::setprecision(2) << added.getPricePerKg()
                << "/kg) has been registered and saved to listings.txt.";
            std::string html = renderRegisterPage(msg.str());
            sendHttpResponse(clientSocket, "200 OK", "text/html", html);
        } else {
            std::string html = renderRegisterPage("Error: Missing or invalid submission inputs. Please verify all required fields.");
            sendHttpResponse(clientSocket, "400 Bad Request", "text/html", html);
        }
    }
    // 5. Available Listings (GET View)
    else if (path == "/listings" && method == "GET") {
        std::string html = renderListingsPage(marketplace);
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 6. Delete Record (POST Action)
    else if (path == "/delete" && method == "POST") {
        auto formData = parseFormData(body);
        int deleteId = 0;
        try {
            if (!formData["id"].empty()) deleteId = std::stoi(formData["id"]);
        } catch (...) {
            deleteId = 0;
        }

        std::string actionMsg = "";
        if (deleteId > 0 && marketplace.deleteListing(deleteId)) {
            actionMsg = "Record #" + std::to_string(deleteId) + " was successfully deleted from inventory and listings.txt.";
        } else {
            actionMsg = "Error: Record #" + std::to_string(deleteId) + " could not be found or deleted.";
        }
        std::string html = renderListingsPage(marketplace, actionMsg);
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 7. Buyer Search (GET View & Query)
    else if (path == "/search" && method == "GET") {
        if (!rawQuery.empty()) {
            auto queryParams = parseFormData(rawQuery);
            std::string mat = queryParams["material"];
            std::string cat = queryParams["category"];
            std::string loc = queryParams["location"];
            double minQty = 0.0;
            try {
                if (!queryParams["minQty"].empty()) minQty = std::stod(queryParams["minQty"]);
            } catch (...) {}
            std::string html = renderSearchPage(marketplace, mat, cat, loc, minQty, true);
            sendHttpResponse(clientSocket, "200 OK", "text/html", html);
        } else {
            std::string html = renderSearchPage(marketplace, "", "", "", 0.0, false);
            sendHttpResponse(clientSocket, "200 OK", "text/html", html);
        }
    }
    // 8. Resource Matcher (GET Form & POST Engine Execution)
    else if (path == "/match" && method == "GET") {
        std::string html = renderMatchPage(marketplace);
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    else if (path == "/match" && method == "POST") {
        auto formData = parseFormData(body);
        std::string targetMat = formData["targetMaterial"];
        std::string prefLoc = formData["preferredLocation"];
        double reqQty = 0.0;
        double maxBudget = 0.0;
        try {
            if (!formData["requiredQuantity"].empty()) reqQty = std::stod(formData["requiredQuantity"]);
            if (!formData["maxBudget"].empty()) maxBudget = std::stod(formData["maxBudget"]);
        } catch (...) {}

        std::string html = renderMatchPage(marketplace, targetMat, reqQty, prefLoc, maxBudget, true);
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 9. About Page
    else if (path == "/about") {
        std::string html = renderAboutPage();
        sendHttpResponse(clientSocket, "200 OK", "text/html", html);
    }
    // 10. Fallback / 404
    else {
        std::string notFound = "<html><body style=\"font-family:sans-serif; text-align:center; padding:3rem;\">"
                               "<h1>404 Not Found</h1><p>Requested resource does not exist on C++ server.</p>"
                               "<p><a href=\"/\">Return to Marketplace Home</a></p></body></html>";
        sendHttpResponse(clientSocket, "404 Not Found", "text/html", notFound);
    }

#ifdef _WIN32
    shutdown(clientSocket, SD_SEND);
#else
    shutdown(clientSocket, SHUT_WR);
#endif
    CLOSE_SOCKET(clientSocket);
}

// ============================================================================
// 6. MAIN FUNCTION: SERVER BOOTSTRAP & EVENT LOOP
// ============================================================================

int main() {
    const char* envPort = std::getenv("PORT");
    int port = envPort ? std::stoi(envPort) : 10000;
    std::cout << "========================================================\n"
              << " Smart Agricultural By-Product Marketplace - C++ Server\n"
              << " Second-Year Computer Science Engineering Mini Project\n"
              << " Built with Zero JavaScript (Pure HTML5, CSS3 & C++)\n"
              << "========================================================\n\n";

    // Initialize Marketplace and Data Persistence
    WasteMarketplace marketplace("listings.txt");
    std::cout << "[INFO] Initialized WasteMarketplace with "
              << marketplace.getTotalCount() << " records ("
              << marketplace.getTotalQuantity() << " kg total biomass).\n";

    // Socket Initialization for Windows
#ifdef _WIN32
    WSADATA wsaData;
    int wsaRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaRes != 0) {
        std::cerr << "[ERROR] WSAStartup failed: " << wsaRes << std::endl;
        return 1;
    }
#endif

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "[ERROR] Failed to create socket." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Allow quick port re-use
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr,
         sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cerr << "[ERROR] Bind failed on port "
              << port << std::endl;
    CLOSE_SOCKET(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 1;
}

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        std::cerr << "[ERROR] Listen failed." << std::endl;
        CLOSE_SOCKET(serverSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::cout << "[SUCCESS] C++ Web Server active!\n"
              << "--------------------------------------------------------\n"
              << " Open your web browser and navigate to:\n"
              << " >>> http://localhost:" << port << "/ <<<\n"
              << "--------------------------------------------------------\n"
              << " Press Ctrl+C in terminal to stop server.\n\n";

    // Server Loop: Accept incoming HTTP connections
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket != INVALID_SOCKET) {
            handleClient(clientSocket, marketplace);
        }
    }

    CLOSE_SOCKET(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
