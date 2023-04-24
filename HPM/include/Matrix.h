#include <utility>
#include <vector>
#include <type_traits>

namespace bombace {

	struct Row {};
	struct Col {};

	template <typename DimIterator>
	class VectorIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename DimIterator::value_type;
		using difference_type = std::ptrdiff_t;
		using pointer = typename DimIterator::pointer;
		using reference = typename DimIterator::reference;
		using dimension = typename DimIterator::dimension;
		VectorIterator(DimIterator& dim, size_t index = 0) : m_vector{dim}, m_index{index} {}
		VectorIterator operator++() { m_index++; return *this; }
		VectorIterator operator++(int) { VectorIterator tmp = *this; ++(*this); return tmp; }
     	friend bool operator== (const VectorIterator& a, const VectorIterator& b) { return (&(a.m_vector) == &(b.m_vector)) && (a.m_index == b.m_index) && (a.m_size == b.m_size); };
		VectorIterator begin() { return VectorIterator(m_vector); }
		VectorIterator end() { return VectorIterator(m_vector, m_size);}

        reference operator*() const {return m_vector.at(m_index);}

		private:
		DimIterator& m_vector;
		size_t m_index;
		size_t m_size;
	};

	template<typename M, typename Dimension>
	class DimIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename M::type;
		using difference_type = std::ptrdiff_t;
		using pointer = typename M::type*;
		using reference = typename M::type&;
		using dimension = Dimension;
		DimIterator(M& m, size_t currentDim = 0) : m_matrix{ m }, m_currentDim{ currentDim } {}
		DimIterator operator++() { m_currentDim++; return *this; }
		DimIterator operator++(int) { DimIterator tmp = *this; ++(*this); return tmp; }
		friend bool operator== (const DimIterator& a, const DimIterator& b) { return (&(a.m_matrix) == &(b.m_matrix)) && (a.m_currentDim == b.m_currentDim); };
		friend bool operator!= (const DimIterator& a, const DimIterator& b) { return (&(a.m_matrix) != &(b.m_matrix)) || (a.m_currentDim != b.m_currentDim); };
		DimIterator begin() { return DimIterator(m_matrix); }
		DimIterator end() {
			if constexpr (std::is_same_v<Dimension, Row>) { return DimIterator(m_matrix, m_matrix.m_dim.first); }
			else if constexpr (std::is_same_v<Dimension, Col>) { return DimIterator(m_matrix, m_matrix.m_dim.second); }
		}

		auto vecItBegin() {return VectorIterator(*this, 0);}

		auto vecItEnd() {
			if constexpr (std::is_same_v<Dimension, Row>) { return VectorIterator(*this, m_matrix.m_dim.second);}
			else if constexpr (std::is_same_v<Dimension, Col>) { return VectorIterator(*this, m_matrix.m_dim.first); }
		}

	private:
	
		friend VectorIterator<DimIterator>;
		
		reference at(size_t i) const {
			if constexpr (std::is_same_v<Dimension, Row>) { return m_matrix.m_vals[m_currentDim + i * (m_matrix.m_dim.first)]; }
			else if constexpr (std::is_same_v<Dimension, Col>) { return m_matrix.m_vals[m_currentDim * m_matrix.m_dim.first + i]; }
		}

		reference at(size_t i) {
			if constexpr (std::is_same_v<Dimension, Row>) { return m_matrix.m_vals[m_currentDim + i * (m_matrix.m_dim.first)]; }
			else if constexpr (std::is_same_v<Dimension, Col>) { return m_matrix.m_vals[m_currentDim * m_matrix.m_dim.first + i]; }
		}

		M& m_matrix;
		size_t m_currentDim = 0;
	};

	template<typename M>
	using RowIterator = DimIterator<M, Row>;

	template <typename M>
	using ColIterator = DimIterator<M, Col>;

	template <typename T, typename StorageDimension = Col>
	class Matrix {
	public:
		using type = T;
		Matrix(std::vector<T>&& vals, size_t rows, size_t cols) : m_vals{ vals }, m_dim{ rows, cols } {}
		const T& at(size_t i, size_t j) const { return m_vals[j * m_dim.first + i]; }
		T& at(size_t i, size_t j) { return m_vals[j * m_dim.first + i]; }
		size_t getRows() const { return m_dim.first; }
		size_t getCols() const { return m_dim.second; }
		auto dimItBegin() {
			if constexpr (std::is_same_v<StorageDimension, Row>) { return RowIterator<Matrix>(*this); }
			else if constexpr (std::is_same_v<StorageDimension, Col>) { return ColIterator<Matrix>(*this); }
		}
		auto dimItEnd() {
			if constexpr (std::is_same_v<StorageDimension, Row>) { return  RowIterator<Matrix>(*this, m_dim.first); }
			else if constexpr (std::is_same_v<StorageDimension, Col>) { return ColIterator<Matrix>(*this, m_dim.second); }
		}
		template <typename U> friend class SubMatrixView;
		template <typename U, typename Dim> friend class DimIterator;
		template <typename U> friend class VectorIterator;

	private:
		std::vector<T> m_vals;
		std::pair<size_t, size_t> m_dim;
	};

	template <typename T>
	class SubMatrixView {
	public:
		SubMatrixView(Matrix<T>& matrix, size_t startRow, size_t startCol, size_t rows, size_t cols) : m_vals{ matrix.m_vals }, m_dim{ rows, cols }, m_ld{ matrix.getRows() }, m_off{ startCol * matrix.getRows() + startRow } {}
		const T& at(size_t i, size_t j) const { return m_vals[m_off + j * m_ld + i]; }
		T& at(size_t i, size_t j) { return m_vals[m_off + j * m_ld + i]; }
		size_t getRows() const { return m_dim.first; }
		size_t getCols() const { return m_dim.second; }
	private:
		size_t m_off;
		std::size_t m_ld;
		std::vector<T>& m_vals;
		std::pair<size_t, size_t> m_dim;
	};
}
